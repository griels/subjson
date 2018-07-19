int compressor::fillDeflate(moku::slice<uint8_t> b)
{
    int n{0};

    if (this->index >= 2 * windowSize - (minMatchLength + maxMatchLength)) {
        copy(this->window,
             moku::slice_expr<moku::slice<uint8_t>>(
                     this->window, windowSize, 2 * windowSize));
        this->index -= windowSize;
        this->windowEnd -= windowSize;
        if (this->blockStart >= windowSize) {
            this->blockStart -= windowSize;
        } else {
            this->blockStart = math::MaxInt32;
        }
        this->hashOffset += windowSize;
        if (this->hashOffset > maxHashOffset) {
            delta = this->hashOffset - 1;
            this->hashOffset -= delta;
            this->chainHead -= delta;
            {
                int i{0};
                uint32_t v{0};
                for (std::tie(i, v) :
                     moku::range_key_value<moku::slice<uint32_t>>(
                             moku::slice_expr<std::vector<uint32_t>>(
                                     this->hashPrev))) {
                    if (int(v) > delta) {
                        this->hashPrev[i] = uint32_t(int(v) - delta);
                    } else {
                        this->hashPrev[i] = 0;
                    }
                }
            }
            {
                int i{0};
                uint32_t v{0};
                for (std::tie(i, v) :
                     moku::range_key_value<moku::slice<uint32_t>>(
                             moku::slice_expr<std::vector<uint32_t>>(
                                     this->hashHead))) {
                    if (int(v) > delta) {
                        this->hashHead[i] = uint32_t(int(v) - delta);
                    } else {
                        this->hashHead[i] = 0;
                    }
                }
            }
        }
    }
    n = copy(moku::slice_expr<moku::slice<uint8_t>>(this->window,
                                                    this->windowEnd),
             b);
    this->windowEnd += n;
    return n;
}
moku::error compressor::writeBlock(moku::slice<token> tokens, int index)
{
    if (index > 0) {
        if (this->blockStart <= index) {
            window = moku::slice_expr<moku::slice<uint8_t>>(
                    this->window, this->blockStart, index);
        }
        this->blockStart = index;
        this->w->writeBlock(tokens, false, window);
        return this->w->err;
    }
    return nil;
}
void compressor::fillWindow(moku::slice<uint8_t> b)
{
    int loops{0};
    int n{0};

    if (this->compressionLevel.level < 2) {
        return;
    }
    if (this->index != 0 || this->windowEnd != 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 4469853) STRING
                     "internal error: fillWindow called with stale data"}));
    }
    if (len(b) > windowSize) {
        b = moku::slice_expr<moku::slice<uint8_t>>(b, len(b) - windowSize);
    }
    n = copy(this->window, b);
    loops = (n + 256 - minMatchLength) / 256;
    {
        int j{0};
        for (j = 0; j < loops; j++) {
            moku::slice<uint32_t> dst{};
            int dstSize{0};
            int end{0};
            int index{0};
            uint32_t newH{0};
            moku::slice<uint8_t> toCheck{};

            index = j * 256;
            end = index + 256 + minMatchLength - 1;
            if (end > n) {
                end = n;
            }
            toCheck = moku::slice_expr<moku::slice<uint8_t>>(
                    this->window, index, end);
            dstSize = len(toCheck) - minMatchLength + 1;
            if (dstSize <= 0) {
                continue;
            }
            dst = moku::slice_expr<std::vector<uint32_t>>(this->hashMatch,
                                                          dstSize);
            this->bulkHasher(toCheck, dst);
            {
                int i{0};
                uint32_t val{0};
                for (std::tie(i, val) :
                     moku::range_key_value<moku::slice<uint32_t>>(dst)) {
                    di = i + index;
                    newH = val;
                    hh = &this->hashHead[newH & hashMask];
                    this->hashPrev[di & windowMask] = *hh;
                    *hh = uint32_t(di + this->hashOffset);
                }
            }
            this->hash = newH;
        }
    }
    this->windowEnd = n;
    this->index = n;
}
std::tuple<int, int, bool> compressor::findMatch(int pos,
                                                 int prevHead,
                                                 int prevLength,
                                                 int lookahead)
{
    int length{0};
    int minIndex{0};
    int minMatchLook{0};
    int nice{0};
    int offset{0};
    bool ok{false};
    int tries{0};
    uint8_t wEnd{0};
    moku::slice<uint8_t> wPos{};
    moku::slice<uint8_t> win{};

    minMatchLook = maxMatchLength;
    if (lookahead < minMatchLook) {
        minMatchLook = lookahead;
    }
    win = moku::slice_expr<moku::slice<uint8_t>>(
            this->window, 0, pos + minMatchLook);
    nice = len(win) - pos;
    if (this->nice < nice) {
        nice = this->nice;
    }
    tries = this->chain;
    length = prevLength;
    if (length >= this->good) {
        tries >>= 2;
    }
    wEnd = win[pos + length];
    wPos = moku::slice_expr<moku::slice<uint8_t>>(win, pos);
    minIndex = pos - windowSize;
    {
        int i{0};
        for (i = prevHead; tries > 0; tries--) {
            if (wEnd == win[i + length]) {
                n = matchLen(moku::slice_expr<moku::slice<uint8_t>>(win, i),
                             wPos,
                             minMatchLook);
                if (n > length && (n > minMatchLength || pos - i <= 4096)) {
                    length = n;
                    offset = pos - i;
                    ok = true;
                    if (n >= nice) {
                        break;
                    }
                    wEnd = win[pos + n];
                }
            }
            if (i == minIndex) {
                break;
            }
            i = int(this->hashPrev[i & windowMask]) - this->hashOffset;
            if (i < minIndex || i < 0) {
                break;
            }
        }
    }
    return;
}
moku::error compressor::writeStoredBlock(moku::slice<uint8_t> buf)
{
    {
        this->w->writeStoredHeader(len(buf), false);

        if (!moku::is_nil(this->w->err)) {
            return this->w->err;
        }
    }
    this->w->writeBytes(buf);
    return this->w->err;
}
uint32_t hash4(moku::slice<uint8_t> b)
{
    return ((uint32_t(b[3]) | uint32_t(b[2]) << 8 | uint32_t(b[1]) << 16 |
             uint32_t(b[0]) << 24) *
            hashmul) >>
           (32 - hashBits);
}
void bulkHash4(moku::slice<uint8_t> b, moku::slice<uint32_t> dst)
{
    int end{0};
    uint32_t hb{0};

    if (len(b) < minMatchLength) {
        return;
    }
    hb = uint32_t(b[3]) | uint32_t(b[2]) << 8 | uint32_t(b[1]) << 16 |
         uint32_t(b[0]) << 24;
    dst[0] = (hb * hashmul) >> (32 - hashBits);
    end = len(b) - minMatchLength + 1;
    {
        int i{0};
        for (i = 1; i < end; i++) {
            hb = (hb << 8) | uint32_t(b[i + 3]);
            dst[i] = (hb * hashmul) >> (32 - hashBits);
        }
    }
}
int matchLen(moku::slice<uint8_t> a, moku::slice<uint8_t> b, int max)
{
    a = moku::slice_expr<moku::slice<uint8_t>>(a, max);
    b = moku::slice_expr<moku::slice<uint8_t>>(b, len(a));
    {
        uint8_t av{0};
        int i{0};
        for (std::tie(i, av) : moku::range_key_value<moku::slice<uint8_t>>(a)) {
            if (b[i] != av) {
                return i;
            }
        }
    }
    return max;
}
void compressor::encSpeed()
{
    if (this->windowEnd < maxStoreBlockSize) {
        if (!this->sync) {
            return;
        }
        if (this->windowEnd < 128) {
            if ((this->windowEnd == 0)) {
            _ident_0_:
                return;
            } else if ((this->windowEnd <= 16)) {
            _ident_1_:
                this->err = this->writeStoredBlock(
                        moku::slice_expr<moku::slice<uint8_t>>(
                                this->window, this->windowEnd));
            } else {
            _ident_2_:
                this->w->writeBlockHuff(false,
                                        moku::slice_expr<moku::slice<uint8_t>>(
                                                this->window, this->windowEnd));
                this->err = this->w->err;
            }
            this->windowEnd = 0;
            this->bestSpeed->reset();
            return;
        }
    }
    this->tokens = this->bestSpeed->encode(
            moku::slice_expr<moku::slice<token>>(this->tokens, 0),
            moku::slice_expr<moku::slice<uint8_t>>(this->window,
                                                   this->windowEnd));
    if (len(this->tokens) > this->windowEnd - (this->windowEnd >> 4)) {
        this->w->writeBlockHuff(false,
                                moku::slice_expr<moku::slice<uint8_t>>(
                                        this->window, this->windowEnd));
    } else {
        this->w->writeBlockDynamic(this->tokens,
                                   false,
                                   moku::slice_expr<moku::slice<uint8_t>>(
                                           this->window, this->windowEnd));
    }
    this->err = this->w->err;
    this->windowEnd = 0;
}
void compressor::initDeflate()
{
    this->window = make(moku::slice<byte>, 2 * windowSize);
    this->hashOffset = 1;
    this->tokens = make(moku::slice<token>, 0, maxFlateBlockTokens + 1);
    this->length = minMatchLength - 1;
    this->offset = 0;
    this->byteAvailable = false;
    this->index = 0;
    this->hash = 0;
    this->chainHead = -1;
    this->bulkHasher = bulkHash4;
}
void compressor::deflate()
{
    if (this->windowEnd - this->index < minMatchLength + maxMatchLength &&
        !this->sync) {
        return;
    }
    this->maxInsertIndex = this->windowEnd - (minMatchLength - 1);
    if (this->index < this->maxInsertIndex) {
        this->hash = hash4(moku::slice_expr<moku::slice<uint8_t>>(
                this->window, this->index, this->index + minMatchLength));
    }
Loop:;
}
int compressor::fillStore(moku::slice<uint8_t> b)
{
    int n{0};

    n = copy(moku::slice_expr<moku::slice<uint8_t>>(this->window,
                                                    this->windowEnd),
             b);
    this->windowEnd += n;
    return n;
}
void compressor::store()
{
    if (this->windowEnd > 0 &&
        (this->windowEnd == maxStoreBlockSize || this->sync)) {
        this->err =
                this->writeStoredBlock(moku::slice_expr<moku::slice<uint8_t>>(
                        this->window, this->windowEnd));
        this->windowEnd = 0;
    }
}
void compressor::storeHuff()
{
    if (this->windowEnd < len(this->window) && !this->sync ||
        this->windowEnd == 0) {
        return;
    }
    this->w->writeBlockHuff(false,
                            moku::slice_expr<moku::slice<uint8_t>>(
                                    this->window, this->windowEnd));
    this->err = this->w->err;
    this->windowEnd = 0;
}
std::tuple<int, moku::error> compressor::write(moku::slice<uint8_t> b)
{
    moku::error err{};
    int n{0};

    if (!moku::is_nil(this->err)) {
        return {0, this->err};
    }
    n = len(b);
    while (len(b) > 0) {
        this->step(this);
        b = moku::slice_expr<moku::slice<uint8_t>>(b, this->fill(this, b));
        if (!moku::is_nil(this->err)) {
            return {0, this->err};
        }
    }
    return {n, nil};
}
moku::error compressor::syncFlush()
{
    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    this->sync = true;
    this->step(this);
    if (moku::is_nil(this->err)) {
        this->w->writeStoredHeader(0, false);
        this->w->flush();
        this->err = this->w->err;
    }
    this->sync = false;
    return this->err;
}
moku::error compressor::init(Writer w, int level)
{
    moku::error err{};

    this->w = newHuffmanBitWriter(w);
    if ((level == NoCompression)) {
    _ident_3_:
        this->window = make(moku::slice<byte>, maxStoreBlockSize);
        this->fill = (*compressor)->fillStore;
        this->step = (*compressor)->store;
    } else if ((level == HuffmanOnly)) {
    _ident_4_:
        this->window = make(moku::slice<byte>, maxStoreBlockSize);
        this->fill = (*compressor)->fillStore;
        this->step = (*compressor)->storeHuff;
    } else if ((level == BestSpeed)) {
    _ident_5_:
        this->compressionLevel = levels[level];
        this->window = make(moku::slice<byte>, maxStoreBlockSize);
        this->fill = (*compressor)->fillStore;
        this->step = (*compressor)->encSpeed;
        this->bestSpeed = newDeflateFast();
        this->tokens = make(moku::slice<token>, maxStoreBlockSize);
    } else if ((level == DefaultCompression)) {
    _ident_6_:
        level = 6;
        goto _ident_7_;
    } else if ((2 <= level && level <= 9)) {
    _ident_7_:
        this->compressionLevel = levels[level];
        this->initDeflate();
        this->fill = (*compressor)->fillDeflate;
        this->step = (*compressor)->deflate;
    } else {
    _ident_8_:
        return fmt::Errorf(
                "flate: invalid compression level %d: want value in range [-2, "
                "9]",
                level);
    }
    return nil;
}
void compressor::reset(Writer w)
{
    this->w->reset(w);
    this->sync = false;
    this->err = nil;
    if ((this->compressionLevel.level == NoCompression)) {
    _ident_9_:
        this->windowEnd = 0;
    } else if ((this->compressionLevel.level == BestSpeed)) {
    _ident_10_:
        this->windowEnd = 0;
        this->tokens = moku::slice_expr<moku::slice<token>>(this->tokens, 0);
        this->bestSpeed->reset();
    } else {
    _ident_11_:
        this->chainHead = -1;
        {
            int i{0};
            for (i : moku::range_key<std::vector<uint32_t>>(this->hashHead)) {
                this->hashHead[i] = 0;
            }
        }
        {
            int i{0};
            for (i : moku::range_key<std::vector<uint32_t>>(this->hashPrev)) {
                this->hashPrev[i] = 0;
            }
        }
        this->hashOffset = 1;
        std::tie(this->index, this->windowEnd) = std::tuple<int, int>(0, 0);
        std::tie(this->blockStart, this->byteAvailable) =
                std::tuple<int, bool>(0, false);
        this->tokens = moku::slice_expr<moku::slice<token>>(this->tokens, 0);
        this->length = minMatchLength - 1;
        this->offset = 0;
        this->hash = 0;
        this->maxInsertIndex = 0;
    }
}
moku::error compressor::close()
{
    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    this->sync = true;
    this->step(this);
    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    {
        this->w->writeStoredHeader(0, true);

        if (!moku::is_nil(this->w->err)) {
            return this->w->err;
        }
    }
    this->w->flush();
    return this->w->err;
}
std::tuple<Writer *, moku::error> NewWriter(Writer w, int level)
{
    Writer dw{};

    {
        moku::error err{};

        err = dw.d.init(w, level);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    return {&dw, nil};
}
std::tuple<Writer *, moku::error> NewWriterDict(Writer w,
                                                int level,
                                                moku::slice<uint8_t> dict)
{
    dictWriter *dw{std::nullptr};
    moku::error err{};
    Writer *zw{std::nullptr};

    dw = &dictWriter{w};
    std::tie(zw, err) = NewWriter(dw, level);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    zw->d.fillWindow(dict);
    zw->dict = append(zw->dict, dict);
    return {zw, err};
}
std::tuple<int, moku::error> dictWriter::Write(moku::slice<uint8_t> b)
{
    moku::error err{};
    int n{0};

    return this->w.Write(b);
}
std::tuple<int, moku::error> Writer::Write(moku::slice<uint8_t> data)
{
    moku::error err{};
    int n{0};

    return this->d.write(data);
}
moku::error Writer::Flush()
{
    return this->d.syncFlush();
}
moku::error Writer::Close()
{
    return this->d.close();
}
void Writer::Reset(Writer dst)
{
    {
        dictWriter *dw{std::nullptr};
        bool ok{false};

        std::tie(dw, ok) = moku::try_assert<*dictWriter>(this->d.w->writer);

        if (ok) {
            dw->w = dst;
            this->d.reset(dw);
            this->d.fillWindow(this->dict);
        } else {
            this->d.reset(dst);
        }
    }
}
void init();
int main()
{
    init();
    levels = moku::slice<compressionLevel>{{0, 0, 0, 0, 0, 0},
                                           {1, 0, 0, 0, 0, 0},
                                           {2, 4, 0, 16, 8, 5},
                                           {3, 4, 0, 32, 32, 6},
                                           {4, 4, 4, 16, 16, skipNever},
                                           {5, 8, 16, 32, 32, skipNever},
                                           {6, 8, 16, 128, 128, skipNever},
                                           {7, 8, 32, 128, 256, skipNever},
                                           {8, 32, 128, 258, 1024, skipNever},
                                           {9, 32, 258, 258, 4096, skipNever}};
    lengthExtraBits =
            moku::slice<int8_t>{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
                                2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
    lengthBase = moku::slice<uint32_t>{
            0,  1,  2,  3,  4,  5,  6,  7,  8,   10,  12,  14,  16,  20, 24,
            28, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 255};
    offsetExtraBits = moku::slice<int8_t>{
            0,  0,  0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,
            6,  7,  7,  8,  8,  9,  9,  10, 10, 11, 11, 12, 12, 13, 13,
            14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20};
    offsetBase = moku::slice<uint32_t>{
            0x000000, 0x000001, 0x000002, 0x000003, 0x000004, 0x000006,
            0x000008, 0x00000c, 0x000010, 0x000018, 0x000020, 0x000030,
            0x000040, 0x000060, 0x000080, 0x0000c0, 0x000100, 0x000180,
            0x000200, 0x000300, 0x000400, 0x000600, 0x000800, 0x000c00,
            0x001000, 0x001800, 0x002000, 0x003000, 0x004000, 0x006000,
            0x008000, 0x00c000, 0x010000, 0x018000, 0x020000, 0x030000,
            0x040000, 0x060000, 0x080000, 0x0c0000, 0x100000, 0x180000,
            0x200000, 0x300000};
    codegenOrder = moku::slice<uint32_t>{
            16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    fixedOffsetEncoding = generateFixedOffsetEncoding();
    fixedLiteralEncoding = generateFixedLiteralEncoding();
    codeOrder = std::vector<int>{
            16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    lengthCodes = std::vector<uint32_t>{
            0,  1,  2,  3,  4,  5,  6,  7,  8,  8,  9,  9,  10, 10, 11, 11,
            12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15,
            16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17,
            18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19,
            20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
            21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
            22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
            23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
            25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
            26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
            26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
            27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
            27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28};
    offsetCodes = std::vector<uint32_t>{
            0,  1,  2,  3,  4,  4,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,
            8,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,
            10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
            11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
            12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
            12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
            13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
            13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
            14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
            14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
            14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
            14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
            15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};
    _main();
    return 0;
}
