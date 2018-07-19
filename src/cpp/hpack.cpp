Encoder *NewEncoder(Writer w)
{
    Encoder *e{std::nullptr};

    e = &Encoder{{minSize, uint32Max},
                 {maxSizeLimit, initialHeaderTableSize},
                 {tableSizeUpdate, false},
                 {w, w}};
    e->dynTab.table.init();
    e->dynTab.setMaxSize(initialHeaderTableSize);
    return e;
}
moku::error Encoder::WriteField(HeaderField f)
{
    moku::error err{};
    uint64_t idx{0};
    int n{0};
    bool nameValueMatch{false};

    this->buf = moku::slice_expr<moku::slice<uint8_t>>(this->buf, 0);
    if (this->tableSizeUpdate) {
        this->tableSizeUpdate = false;
        if (this->minSize < this->dynTab.maxSize) {
            this->buf = appendTableSize(this->buf, this->minSize);
        }
        this->minSize = uint32Max;
        this->buf = appendTableSize(this->buf, this->dynTab.maxSize);
    }
    std::tie(idx, nameValueMatch) = this->searchTable(f);
    if (nameValueMatch) {
        this->buf = appendIndexed(this->buf, idx);
    } else {
        indexing = this->shouldIndex(f);
        if (indexing) {
            this->dynTab.add(f);
        }
        if (idx == 0) {
            this->buf = appendNewName(this->buf, f, indexing);
        } else {
            this->buf = appendIndexedName(this->buf, f, idx, indexing);
        }
    }
    std::tie(n, err) = this->w.Write(this->buf);
    if (moku::is_nil(err) && n != len(this->buf)) {
        err = io::ErrShortWrite;
    }
    return err;
}
std::tuple<uint64_t, bool> Encoder::searchTable(HeaderField f)
{
    uint64_t i{0};
    uint64_t j{0};
    bool nameValueMatch{false};

    std::tie(i, nameValueMatch) = staticTable->search(f);
    if (nameValueMatch) {
        return {i, true};
    }
    std::tie(j, nameValueMatch) = this->dynTab.table.search(f);
    if (nameValueMatch || (i == 0 && j != 0)) {
        return {j + uint64_t(staticTable->len()), nameValueMatch};
    }
    return {i, false};
}
void Encoder::SetMaxDynamicTableSize(uint32_t v)
{
    if (v > this->maxSizeLimit) {
        v = this->maxSizeLimit;
    }
    if (v < this->minSize) {
        this->minSize = v;
    }
    this->tableSizeUpdate = true;
    this->dynTab.setMaxSize(v);
}
void Encoder::SetMaxDynamicTableSizeLimit(uint32_t v)
{
    this->maxSizeLimit = v;
    if (this->dynTab.maxSize > v) {
        this->tableSizeUpdate = true;
        this->dynTab.setMaxSize(v);
    }
}
bool Encoder::shouldIndex(HeaderField f)
{
    return !f.Sensitive && f.Size() <= this->dynTab.maxSize;
}
moku::slice<uint8_t> appendIndexed(moku::slice<uint8_t> dst, uint64_t i)
{
    int first{0};

    first = len(dst);
    dst = appendVarInt(dst, 7, i);
    dst[first] |= 0x80;
    return dst;
}
moku::slice<uint8_t> appendNewName(moku::slice<uint8_t> dst,
                                   HeaderField f,
                                   bool indexing)
{
    dst = append(dst, encodeTypeByte(indexing, f.Sensitive));
    dst = appendHpackString(dst, f.Name);
    return appendHpackString(dst, f.Value);
}
moku::slice<uint8_t> appendIndexedName(moku::slice<uint8_t> dst,
                                       HeaderField f,
                                       uint64_t i,
                                       bool indexing)
{
    int first{0};
    uint8_t n{0};

    first = len(dst);
    if (indexing) {
        n = 6;
    } else {
        n = 4;
    }
    dst = appendVarInt(dst, n, i);
    dst[first] |= encodeTypeByte(indexing, f.Sensitive);
    return appendHpackString(dst, f.Value);
}
moku::slice<uint8_t> appendTableSize(moku::slice<uint8_t> dst, uint32_t v)
{
    int first{0};

    first = len(dst);
    dst = appendVarInt(dst, 5, uint64_t(v));
    dst[first] |= 0x20;
    return dst;
}
moku::slice<uint8_t> appendVarInt(moku::slice<uint8_t> dst,
                                  uint8_t n,
                                  uint64_t i)
{
    uint64_t k{0};

    k = uint64_t((1 << n) - 1);
    if (i < k) {
        return append(dst, byte(i));
    }
    dst = append(dst, byte(k));
    i -= k;
    for (; i >= 128; i >>= 7) {
        dst = append(dst, byte(0x80 | (i & 0x7f)));
    }
    return append(dst, byte(i));
}
moku::slice<uint8_t> appendHpackString(moku::slice<uint8_t> dst, std::string s)
{
    uint64_t huffmanLength{0};

    huffmanLength = HuffmanEncodeLength(s);
    if (huffmanLength < uint64_t(len(s))) {
        first = len(dst);
        dst = appendVarInt(dst, 7, huffmanLength);
        dst = AppendHuffmanString(dst, s);
        dst[first] |= 0x80;
    } else {
        dst = appendVarInt(dst, 7, uint64_t(len(s)));
        dst = append(dst, s);
    }
    return dst;
}
uint8_t encodeTypeByte(bool indexing, bool sensitive)
{
    if (sensitive) {
        return 0x10;
    }
    if (indexing) {
        return 0x40;
    }
    return 0;
}
void init();
int main()
{
    init();
    ErrStringLength = errors::New("hpack: string too long");
    errNeedMore = errors::New("need more data");
    errVarintOverflow = DecodingError{errors::New("varint integer overflow")};
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidHuffman = errors::New("hpack: invalid Huffman-encoded data");
    rootHuffmanNode = newInternalNode();
    staticTableEntries = std::vector<HeaderField>{
            HeaderField{{Name, ":authority"}},
            HeaderField{{Name, ":method"}, {Value, "GET"}},
            HeaderField{{Name, ":method"}, {Value, "POST"}},
            HeaderField{{Name, ":path"}, {Value, "/"}},
            HeaderField{{Name, ":path"}, {Value, "/index.html"}},
            HeaderField{{Name, ":scheme"}, {Value, "http"}},
            HeaderField{{Name, ":scheme"}, {Value, "https"}},
            HeaderField{{Name, ":status"}, {Value, "200"}},
            HeaderField{{Name, ":status"}, {Value, "204"}},
            HeaderField{{Name, ":status"}, {Value, "206"}},
            HeaderField{{Name, ":status"}, {Value, "304"}},
            HeaderField{{Name, ":status"}, {Value, "400"}},
            HeaderField{{Name, ":status"}, {Value, "404"}},
            HeaderField{{Name, ":status"}, {Value, "500"}},
            HeaderField{{Name, "accept-charset"}},
            HeaderField{{Name, "accept-encoding"}, {Value, "gzip, deflate"}},
            HeaderField{{Name, "accept-language"}},
            HeaderField{{Name, "accept-ranges"}},
            HeaderField{{Name, "accept"}},
            HeaderField{{Name, "access-control-allow-origin"}},
            HeaderField{{Name, "age"}},
            HeaderField{{Name, "allow"}},
            HeaderField{{Name, "authorization"}},
            HeaderField{{Name, "cache-control"}},
            HeaderField{{Name, "content-disposition"}},
            HeaderField{{Name, "content-encoding"}},
            HeaderField{{Name, "content-language"}},
            HeaderField{{Name, "content-length"}},
            HeaderField{{Name, "content-location"}},
            HeaderField{{Name, "content-range"}},
            HeaderField{{Name, "content-type"}},
            HeaderField{{Name, "cookie"}},
            HeaderField{{Name, "date"}},
            HeaderField{{Name, "etag"}},
            HeaderField{{Name, "expect"}},
            HeaderField{{Name, "expires"}},
            HeaderField{{Name, "from"}},
            HeaderField{{Name, "host"}},
            HeaderField{{Name, "if-match"}},
            HeaderField{{Name, "if-modified-since"}},
            HeaderField{{Name, "if-none-match"}},
            HeaderField{{Name, "if-range"}},
            HeaderField{{Name, "if-unmodified-since"}},
            HeaderField{{Name, "last-modified"}},
            HeaderField{{Name, "link"}},
            HeaderField{{Name, "location"}},
            HeaderField{{Name, "max-forwards"}},
            HeaderField{{Name, "proxy-authenticate"}},
            HeaderField{{Name, "proxy-authorization"}},
            HeaderField{{Name, "range"}},
            HeaderField{{Name, "referer"}},
            HeaderField{{Name, "refresh"}},
            HeaderField{{Name, "retry-after"}},
            HeaderField{{Name, "server"}},
            HeaderField{{Name, "set-cookie"}},
            HeaderField{{Name, "strict-transport-security"}},
            HeaderField{{Name, "transfer-encoding"}},
            HeaderField{{Name, "user-agent"}},
            HeaderField{{Name, "vary"}},
            HeaderField{{Name, "via"}},
            HeaderField{{Name, "www-authenticate"}}};
    staticTable = newStaticTable();
    huffmanCodes = std::vector<uint32_t>{
            0x1ff8,    0x7fffd8,   0xfffffe2, 0xfffffe3, 0xfffffe4,  0xfffffe5,
            0xfffffe6, 0xfffffe7,  0xfffffe8, 0xffffea,  0x3ffffffc, 0xfffffe9,
            0xfffffea, 0x3ffffffd, 0xfffffeb, 0xfffffec, 0xfffffed,  0xfffffee,
            0xfffffef, 0xffffff0,  0xffffff1, 0xffffff2, 0x3ffffffe, 0xffffff3,
            0xffffff4, 0xffffff5,  0xffffff6, 0xffffff7, 0xffffff8,  0xffffff9,
            0xffffffa, 0xffffffb,  0x14,      0x3f8,     0x3f9,      0xffa,
            0x1ff9,    0x15,       0xf8,      0x7fa,     0x3fa,      0x3fb,
            0xf9,      0x7fb,      0xfa,      0x16,      0x17,       0x18,
            0x0,       0x1,        0x2,       0x19,      0x1a,       0x1b,
            0x1c,      0x1d,       0x1e,      0x1f,      0x5c,       0xfb,
            0x7ffc,    0x20,       0xffb,     0x3fc,     0x1ffa,     0x21,
            0x5d,      0x5e,       0x5f,      0x60,      0x61,       0x62,
            0x63,      0x64,       0x65,      0x66,      0x67,       0x68,
            0x69,      0x6a,       0x6b,      0x6c,      0x6d,       0x6e,
            0x6f,      0x70,       0x71,      0x72,      0xfc,       0x73,
            0xfd,      0x1ffb,     0x7fff0,   0x1ffc,    0x3ffc,     0x22,
            0x7ffd,    0x3,        0x23,      0x4,       0x24,       0x5,
            0x25,      0x26,       0x27,      0x6,       0x74,       0x75,
            0x28,      0x29,       0x2a,      0x7,       0x2b,       0x76,
            0x2c,      0x8,        0x9,       0x2d,      0x77,       0x78,
            0x79,      0x7a,       0x7b,      0x7ffe,    0x7fc,      0x3ffd,
            0x1ffd,    0xffffffc,  0xfffe6,   0x3fffd2,  0xfffe7,    0xfffe8,
            0x3fffd3,  0x3fffd4,   0x3fffd5,  0x7fffd9,  0x3fffd6,   0x7fffda,
            0x7fffdb,  0x7fffdc,   0x7fffdd,  0x7fffde,  0xffffeb,   0x7fffdf,
            0xffffec,  0xffffed,   0x3fffd7,  0x7fffe0,  0xffffee,   0x7fffe1,
            0x7fffe2,  0x7fffe3,   0x7fffe4,  0x1fffdc,  0x3fffd8,   0x7fffe5,
            0x3fffd9,  0x7fffe6,   0x7fffe7,  0xffffef,  0x3fffda,   0x1fffdd,
            0xfffe9,   0x3fffdb,   0x3fffdc,  0x7fffe8,  0x7fffe9,   0x1fffde,
            0x7fffea,  0x3fffdd,   0x3fffde,  0xfffff0,  0x1fffdf,   0x3fffdf,
            0x7fffeb,  0x7fffec,   0x1fffe0,  0x1fffe1,  0x3fffe0,   0x1fffe2,
            0x7fffed,  0x3fffe1,   0x7fffee,  0x7fffef,  0xfffea,    0x3fffe2,
            0x3fffe3,  0x3fffe4,   0x7ffff0,  0x3fffe5,  0x3fffe6,   0x7ffff1,
            0x3ffffe0, 0x3ffffe1,  0xfffeb,   0x7fff1,   0x3fffe7,   0x7ffff2,
            0x3fffe8,  0x1ffffec,  0x3ffffe2, 0x3ffffe3, 0x3ffffe4,  0x7ffffde,
            0x7ffffdf, 0x3ffffe5,  0xfffff1,  0x1ffffed, 0x7fff2,    0x1fffe3,
            0x3ffffe6, 0x7ffffe0,  0x7ffffe1, 0x3ffffe7, 0x7ffffe2,  0xfffff2,
            0x1fffe4,  0x1fffe5,   0x3ffffe8, 0x3ffffe9, 0xffffffd,  0x7ffffe3,
            0x7ffffe4, 0x7ffffe5,  0xfffec,   0xfffff3,  0xfffed,    0x1fffe6,
            0x3fffe9,  0x1fffe7,   0x1fffe8,  0x7ffff3,  0x3fffea,   0x3fffeb,
            0x1ffffee, 0x1ffffef,  0xfffff4,  0xfffff5,  0x3ffffea,  0x7ffff4,
            0x3ffffeb, 0x7ffffe6,  0x3ffffec, 0x3ffffed, 0x7ffffe7,  0x7ffffe8,
            0x7ffffe9, 0x7ffffea,  0x7ffffeb, 0xffffffe, 0x7ffffec,  0x7ffffed,
            0x7ffffee, 0x7ffffef,  0x7fffff0, 0x3ffffee};
    huffmanCodeLen = std::vector<uint8_t>{
            13, 23, 28, 28, 28, 28, 28, 28, 28, 24, 30, 28, 28, 30, 28, 28,
            28, 28, 28, 28, 28, 28, 30, 28, 28, 28, 28, 28, 28, 28, 28, 28,
            6,  10, 10, 12, 13, 6,  8,  11, 10, 10, 8,  11, 8,  6,  6,  6,
            5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  7,  8,  15, 6,  12, 10,
            13, 6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
            7,  7,  7,  7,  7,  7,  7,  7,  8,  7,  8,  13, 19, 13, 14, 6,
            15, 5,  6,  5,  6,  5,  6,  6,  6,  5,  7,  7,  6,  6,  6,  5,
            6,  7,  6,  5,  5,  6,  7,  7,  7,  7,  7,  15, 11, 14, 13, 28,
            20, 22, 20, 20, 22, 22, 22, 23, 22, 23, 23, 23, 23, 23, 24, 23,
            24, 24, 22, 23, 24, 23, 23, 23, 23, 21, 22, 23, 22, 23, 23, 24,
            22, 21, 20, 22, 22, 23, 23, 21, 23, 22, 22, 24, 21, 22, 23, 23,
            21, 21, 22, 21, 23, 22, 23, 23, 20, 22, 22, 22, 23, 22, 22, 23,
            26, 26, 20, 19, 22, 23, 22, 25, 26, 26, 26, 27, 27, 26, 24, 25,
            19, 21, 26, 27, 27, 26, 27, 24, 21, 21, 26, 26, 28, 27, 27, 27,
            20, 24, 20, 21, 22, 21, 21, 23, 22, 22, 25, 25, 24, 24, 26, 23,
            26, 27, 26, 26, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 26};
    _main();
    return 0;
}
std::string DecodingError::_ErrorByValue()
{
    return fmt::Sprintf("decoding error: %v", this->Err);
}
std::string InvalidIndexError::_ErrorByValue()
{
    return fmt::Sprintf("invalid indexed representation index %d", int(this));
}
bool HeaderField::_IsPseudoByValue()
{
    return len(this->Name) != 0 && this->Name[0] == ':';
}
std::string HeaderField::_StringByValue()
{
    std::string suffix{""};

    if (this->Sensitive) {
        suffix = " (sensitive)";
    }
    return fmt::Sprintf(
            "header field %q = %q%s", this->Name, this->Value, suffix);
}
uint32_t HeaderField::_SizeByValue()
{
    return uint32_t(len(this->Name) + len(this->Value) + 32);
}
Decoder *NewDecoder(uint32_t maxDynamicTableSize,
                    std::function<void(HeaderField)> emitFunc)
{
    Decoder *d{std::nullptr};

    d = &Decoder{{emit, emitFunc}, {emitEnabled, true}};
    d->dynTab.table.init();
    d->dynTab.allowedMaxSize = maxDynamicTableSize;
    d->dynTab.setMaxSize(maxDynamicTableSize);
    return d;
}
void Decoder::SetMaxStringLength(int n)
{
    this->maxStrLen = n;
}
void Decoder::SetEmitFunc(std::function<void(HeaderField)> emitFunc)
{
    this->emit = emitFunc;
}
void Decoder::SetEmitEnabled(bool v)
{
    this->emitEnabled = v;
}
bool Decoder::EmitEnabled()
{
    return this->emitEnabled;
}
void Decoder::SetMaxDynamicTableSize(uint32_t v)
{
    this->dynTab.setMaxSize(v);
}
void Decoder::SetAllowedMaxDynamicTableSize(uint32_t v)
{
    this->dynTab.allowedMaxSize = v;
}
void dynamicTable::setMaxSize(uint32_t v)
{
    this->maxSize = v;
    this->evict();
}
void dynamicTable::add(HeaderField f)
{
    this->table.addEntry(f);
    this->size += f.Size();
    this->evict();
}
void dynamicTable::evict()
{
    int n{0};

    while (this->size > this->maxSize && n < this->table.len()) {
        this->size -= this->table.ents[n].Size();
        n++;
    }
    this->table.evictOldest(n);
}
int Decoder::maxTableIndex()
{
    return this->dynTab.table.len() + staticTable->len();
}
std::tuple<HeaderField, bool> Decoder::at(uint64_t i)
{
    headerFieldTable dt{};
    HeaderField hf{};
    bool ok{false};

    if (i == 0) {
        return;
    }
    if (i <= uint64_t(staticTable->len())) {
        return {staticTable->ents[i - 1], true};
    }
    if (i > uint64_t(this->maxTableIndex())) {
        return;
    }
    dt = this->dynTab.table;
    return {dt.ents[dt.len() - (int(i) - staticTable->len())], true};
}
std::tuple<moku::slice<HeaderField>, moku::error> Decoder::DecodeFull(
        moku::slice<uint8_t> p)
{
    moku::defer _defer_;
    moku::slice<HeaderField> hf{};
    std::function<void(HeaderField)> saveFunc{std::nullptr};

    saveFunc = this->emit;
    _defer_.Push([=]() mutable {
        [=]() mutable -> void { this->emit = saveFunc; }();
    });
    this->emit = [=](HeaderField f) mutable -> void { hf = append(hf, f); };
    {
        moku::error err{};

        std::tie(_, err) = this->Write(p);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    {
        moku::error err{};

        err = this->Close();

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    return {hf, nil};
}
moku::error Decoder::Close()
{
    if (this->saveBuf.Len() > 0) {
        this->saveBuf.Reset();
        return DecodingError{errors::New("truncated headers")};
    }
    return nil;
}
std::tuple<int, moku::error> Decoder::Write(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    if (len(p) == 0) {
        return;
    }
    if (this->saveBuf.Len() == 0) {
        this->buf = p;
    } else {
        this->saveBuf.Write(p);
        this->buf = this->saveBuf.Bytes();
        this->saveBuf.Reset();
    }
    while (len(this->buf) > 0) {
        err = this->parseHeaderFieldRepr();
        if (err == errNeedMore) {
            if (this->maxStrLen != 0 &&
                int64_t(len(this->buf)) >
                        2 * (int64_t(this->maxStrLen) + varIntOverhead)) {
                return {0, ErrStringLength};
            }
            this->saveBuf.Write(this->buf);
            return {len(p), nil};
        }
        if (!moku::is_nil(err)) {
            break;
        }
    }
    return {len(p), err};
}
bool indexType::_indexedByValue()
{
    return this == indexedTrue;
}
bool indexType::_sensitiveByValue()
{
    return this == indexedNever;
}
moku::error Decoder::parseHeaderFieldRepr()
{
    uint8_t b{0};

    b = this->buf[0];
    if ((b & 128 != 0)) {
    _ident_0_:
        return this->parseFieldIndexed();
    } else if ((b & 192 == 64)) {
    _ident_1_:
        return this->parseFieldLiteral(6, indexedTrue);
    } else if ((b & 240 == 0)) {
    _ident_2_:
        return this->parseFieldLiteral(4, indexedFalse);
    } else if ((b & 240 == 16)) {
    _ident_3_:
        return this->parseFieldLiteral(4, indexedNever);
    } else if ((b & 224 == 32)) {
    _ident_4_:
        return this->parseDynamicTableSizeUpdate();
    }
    return DecodingError{errors::New("invalid encoding")};
}
moku::error Decoder::parseFieldIndexed()
{
    moku::slice<uint8_t> buf{};
    moku::error err{};
    HeaderField hf{};
    uint64_t idx{0};
    bool ok{false};

    buf = this->buf;
    std::tie(idx, buf, err) = readVarInt(7, buf);
    if (!moku::is_nil(err)) {
        return err;
    }
    std::tie(hf, ok) = this->at(idx);
    if (!ok) {
        return DecodingError{InvalidIndexError(idx)};
    }
    this->buf = buf;
    return this->callEmit(HeaderField{{Name, hf.Name}, {Value, hf.Value}});
}
moku::error Decoder::parseFieldLiteral(uint8_t n, indexType it)
{
    moku::slice<uint8_t> buf{};
    moku::error err{};
    HeaderField hf{};
    uint64_t nameIdx{0};
    bool wantStr{false};

    buf = this->buf;
    std::tie(nameIdx, buf, err) = readVarInt(n, buf);
    if (!moku::is_nil(err)) {
        return err;
    }
    wantStr = this->emitEnabled || it.indexed();
    if (nameIdx > 0) {
        std::tie(ihf, ok) = this->at(nameIdx);
        if (!ok) {
            return DecodingError{InvalidIndexError(nameIdx)};
        }
        hf.Name = ihf.Name;
    } else {
        std::tie(hf.Name, buf, err) = this->readString(buf, wantStr);
        if (!moku::is_nil(err)) {
            return err;
        }
    }
    std::tie(hf.Value, buf, err) = this->readString(buf, wantStr);
    if (!moku::is_nil(err)) {
        return err;
    }
    this->buf = buf;
    if (it.indexed()) {
        this->dynTab.add(hf);
    }
    hf.Sensitive = it.sensitive();
    return this->callEmit(hf);
}
moku::error Decoder::callEmit(HeaderField hf)
{
    if (this->maxStrLen != 0) {
        if (len(hf.Name) > this->maxStrLen || len(hf.Value) > this->maxStrLen) {
            return ErrStringLength;
        }
    }
    if (this->emitEnabled) {
        this->emit(hf);
    }
    return nil;
}
moku::error Decoder::parseDynamicTableSizeUpdate()
{
    moku::slice<uint8_t> buf{};
    moku::error err{};
    uint64_t size{0};

    buf = this->buf;
    std::tie(size, buf, err) = readVarInt(5, buf);
    if (!moku::is_nil(err)) {
        return err;
    }
    if (size > uint64_t(this->dynTab.allowedMaxSize)) {
        return DecodingError{
                errors::New("dynamic table size update too large")};
    }
    this->dynTab.setMaxSize(uint32_t(size));
    this->buf = buf;
    return nil;
}
std::tuple<uint64_t, moku::slice<uint8_t>, moku::error> readVarInt(
        uint8_t n, moku::slice<uint8_t> p)
{
    moku::error err{};
    uint64_t i{0};
    uint64_t m{0};
    moku::slice<uint8_t> origP{};
    moku::slice<uint8_t> remain{};

    if (n < 1 || n > 8) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 4997283) STRING "bad n"}));
    }
    if (len(p) == 0) {
        return {0, p, errNeedMore};
    }
    i = uint64_t(p[0]);
    if (n < 8) {
        i &= (1 << uint64_t(n)) - 1;
    }
    if (i < (1 << uint64_t(n)) - 1) {
        return {i, moku::slice_expr<moku::slice<uint8_t>>(p, 1), nil};
    }
    origP = p;
    p = moku::slice_expr<moku::slice<uint8_t>>(p, 1);
    while (len(p) > 0) {
        uint8_t b{0};

        b = p[0];
        p = moku::slice_expr<moku::slice<uint8_t>>(p, 1);
        i += uint64_t(b & 127) << m;
        if (b & 128 == 0) {
            return {i, p, nil};
        }
        m += 7;
        if (m >= 63) {
            return {0, origP, errVarintOverflow};
        }
    }
    return {0, origP, errNeedMore};
}
std::tuple<std::string, moku::slice<uint8_t>, moku::error> Decoder::readString(
        moku::slice<uint8_t> p, bool wantStr)
{
    moku::defer _defer_;
    moku::error err{};
    bool isHuff{false};
    moku::slice<uint8_t> remain{};
    std::string s{""};
    uint64_t strLen{0};

    if (len(p) == 0) {
        return {"", p, errNeedMore};
    }
    isHuff = p[0] & 128 != 0;
    std::tie(strLen, p, err) = readVarInt(7, p);
    if (!moku::is_nil(err)) {
        return {"", p, err};
    }
    if (this->maxStrLen != 0 && strLen > uint64_t(this->maxStrLen)) {
        return {"", nil, ErrStringLength};
    }
    if (uint64_t(len(p)) < strLen) {
        return {"", p, errNeedMore};
    }
    if (!isHuff) {
        if (wantStr) {
            s = std::string(moku::slice_expr<moku::slice<uint8_t>>(p, strLen));
        }
        return {s, moku::slice_expr<moku::slice<uint8_t>>(p, strLen), nil};
    }
    if (wantStr) {
        buf = moku::type_assert<*bytes::Buffer>(bufPool.Get());
        buf->Reset();
        _defer_.Push([=]() mutable {
            bufPool.Put(moku::make_iface<Buffer *>(buf));
        });
        {
            moku::error err{};

            err = huffmanDecode(
                    buf,
                    this->maxStrLen,
                    moku::slice_expr<moku::slice<uint8_t>>(p, strLen));

            if (!moku::is_nil(err)) {
                buf->Reset();
                return {"", nil, err};
            }
        }
        s = buf->String();
        buf->Reset();
    }
    return {s, moku::slice_expr<moku::slice<uint8_t>>(p, strLen), nil};
}
void init();
int main()
{
    init();
    ErrStringLength = errors::New("hpack: string too long");
    errNeedMore = errors::New("need more data");
    errVarintOverflow = DecodingError{errors::New("varint integer overflow")};
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidHuffman = errors::New("hpack: invalid Huffman-encoded data");
    rootHuffmanNode = newInternalNode();
    staticTableEntries = std::vector<HeaderField>{
            HeaderField{{Name, ":authority"}},
            HeaderField{{Name, ":method"}, {Value, "GET"}},
            HeaderField{{Name, ":method"}, {Value, "POST"}},
            HeaderField{{Name, ":path"}, {Value, "/"}},
            HeaderField{{Name, ":path"}, {Value, "/index.html"}},
            HeaderField{{Name, ":scheme"}, {Value, "http"}},
            HeaderField{{Name, ":scheme"}, {Value, "https"}},
            HeaderField{{Name, ":status"}, {Value, "200"}},
            HeaderField{{Name, ":status"}, {Value, "204"}},
            HeaderField{{Name, ":status"}, {Value, "206"}},
            HeaderField{{Name, ":status"}, {Value, "304"}},
            HeaderField{{Name, ":status"}, {Value, "400"}},
            HeaderField{{Name, ":status"}, {Value, "404"}},
            HeaderField{{Name, ":status"}, {Value, "500"}},
            HeaderField{{Name, "accept-charset"}},
            HeaderField{{Name, "accept-encoding"}, {Value, "gzip, deflate"}},
            HeaderField{{Name, "accept-language"}},
            HeaderField{{Name, "accept-ranges"}},
            HeaderField{{Name, "accept"}},
            HeaderField{{Name, "access-control-allow-origin"}},
            HeaderField{{Name, "age"}},
            HeaderField{{Name, "allow"}},
            HeaderField{{Name, "authorization"}},
            HeaderField{{Name, "cache-control"}},
            HeaderField{{Name, "content-disposition"}},
            HeaderField{{Name, "content-encoding"}},
            HeaderField{{Name, "content-language"}},
            HeaderField{{Name, "content-length"}},
            HeaderField{{Name, "content-location"}},
            HeaderField{{Name, "content-range"}},
            HeaderField{{Name, "content-type"}},
            HeaderField{{Name, "cookie"}},
            HeaderField{{Name, "date"}},
            HeaderField{{Name, "etag"}},
            HeaderField{{Name, "expect"}},
            HeaderField{{Name, "expires"}},
            HeaderField{{Name, "from"}},
            HeaderField{{Name, "host"}},
            HeaderField{{Name, "if-match"}},
            HeaderField{{Name, "if-modified-since"}},
            HeaderField{{Name, "if-none-match"}},
            HeaderField{{Name, "if-range"}},
            HeaderField{{Name, "if-unmodified-since"}},
            HeaderField{{Name, "last-modified"}},
            HeaderField{{Name, "link"}},
            HeaderField{{Name, "location"}},
            HeaderField{{Name, "max-forwards"}},
            HeaderField{{Name, "proxy-authenticate"}},
            HeaderField{{Name, "proxy-authorization"}},
            HeaderField{{Name, "range"}},
            HeaderField{{Name, "referer"}},
            HeaderField{{Name, "refresh"}},
            HeaderField{{Name, "retry-after"}},
            HeaderField{{Name, "server"}},
            HeaderField{{Name, "set-cookie"}},
            HeaderField{{Name, "strict-transport-security"}},
            HeaderField{{Name, "transfer-encoding"}},
            HeaderField{{Name, "user-agent"}},
            HeaderField{{Name, "vary"}},
            HeaderField{{Name, "via"}},
            HeaderField{{Name, "www-authenticate"}}};
    staticTable = newStaticTable();
    huffmanCodes = std::vector<uint32_t>{
            0x1ff8,    0x7fffd8,   0xfffffe2, 0xfffffe3, 0xfffffe4,  0xfffffe5,
            0xfffffe6, 0xfffffe7,  0xfffffe8, 0xffffea,  0x3ffffffc, 0xfffffe9,
            0xfffffea, 0x3ffffffd, 0xfffffeb, 0xfffffec, 0xfffffed,  0xfffffee,
            0xfffffef, 0xffffff0,  0xffffff1, 0xffffff2, 0x3ffffffe, 0xffffff3,
            0xffffff4, 0xffffff5,  0xffffff6, 0xffffff7, 0xffffff8,  0xffffff9,
            0xffffffa, 0xffffffb,  0x14,      0x3f8,     0x3f9,      0xffa,
            0x1ff9,    0x15,       0xf8,      0x7fa,     0x3fa,      0x3fb,
            0xf9,      0x7fb,      0xfa,      0x16,      0x17,       0x18,
            0x0,       0x1,        0x2,       0x19,      0x1a,       0x1b,
            0x1c,      0x1d,       0x1e,      0x1f,      0x5c,       0xfb,
            0x7ffc,    0x20,       0xffb,     0x3fc,     0x1ffa,     0x21,
            0x5d,      0x5e,       0x5f,      0x60,      0x61,       0x62,
            0x63,      0x64,       0x65,      0x66,      0x67,       0x68,
            0x69,      0x6a,       0x6b,      0x6c,      0x6d,       0x6e,
            0x6f,      0x70,       0x71,      0x72,      0xfc,       0x73,
            0xfd,      0x1ffb,     0x7fff0,   0x1ffc,    0x3ffc,     0x22,
            0x7ffd,    0x3,        0x23,      0x4,       0x24,       0x5,
            0x25,      0x26,       0x27,      0x6,       0x74,       0x75,
            0x28,      0x29,       0x2a,      0x7,       0x2b,       0x76,
            0x2c,      0x8,        0x9,       0x2d,      0x77,       0x78,
            0x79,      0x7a,       0x7b,      0x7ffe,    0x7fc,      0x3ffd,
            0x1ffd,    0xffffffc,  0xfffe6,   0x3fffd2,  0xfffe7,    0xfffe8,
            0x3fffd3,  0x3fffd4,   0x3fffd5,  0x7fffd9,  0x3fffd6,   0x7fffda,
            0x7fffdb,  0x7fffdc,   0x7fffdd,  0x7fffde,  0xffffeb,   0x7fffdf,
            0xffffec,  0xffffed,   0x3fffd7,  0x7fffe0,  0xffffee,   0x7fffe1,
            0x7fffe2,  0x7fffe3,   0x7fffe4,  0x1fffdc,  0x3fffd8,   0x7fffe5,
            0x3fffd9,  0x7fffe6,   0x7fffe7,  0xffffef,  0x3fffda,   0x1fffdd,
            0xfffe9,   0x3fffdb,   0x3fffdc,  0x7fffe8,  0x7fffe9,   0x1fffde,
            0x7fffea,  0x3fffdd,   0x3fffde,  0xfffff0,  0x1fffdf,   0x3fffdf,
            0x7fffeb,  0x7fffec,   0x1fffe0,  0x1fffe1,  0x3fffe0,   0x1fffe2,
            0x7fffed,  0x3fffe1,   0x7fffee,  0x7fffef,  0xfffea,    0x3fffe2,
            0x3fffe3,  0x3fffe4,   0x7ffff0,  0x3fffe5,  0x3fffe6,   0x7ffff1,
            0x3ffffe0, 0x3ffffe1,  0xfffeb,   0x7fff1,   0x3fffe7,   0x7ffff2,
            0x3fffe8,  0x1ffffec,  0x3ffffe2, 0x3ffffe3, 0x3ffffe4,  0x7ffffde,
            0x7ffffdf, 0x3ffffe5,  0xfffff1,  0x1ffffed, 0x7fff2,    0x1fffe3,
            0x3ffffe6, 0x7ffffe0,  0x7ffffe1, 0x3ffffe7, 0x7ffffe2,  0xfffff2,
            0x1fffe4,  0x1fffe5,   0x3ffffe8, 0x3ffffe9, 0xffffffd,  0x7ffffe3,
            0x7ffffe4, 0x7ffffe5,  0xfffec,   0xfffff3,  0xfffed,    0x1fffe6,
            0x3fffe9,  0x1fffe7,   0x1fffe8,  0x7ffff3,  0x3fffea,   0x3fffeb,
            0x1ffffee, 0x1ffffef,  0xfffff4,  0xfffff5,  0x3ffffea,  0x7ffff4,
            0x3ffffeb, 0x7ffffe6,  0x3ffffec, 0x3ffffed, 0x7ffffe7,  0x7ffffe8,
            0x7ffffe9, 0x7ffffea,  0x7ffffeb, 0xffffffe, 0x7ffffec,  0x7ffffed,
            0x7ffffee, 0x7ffffef,  0x7fffff0, 0x3ffffee};
    huffmanCodeLen = std::vector<uint8_t>{
            13, 23, 28, 28, 28, 28, 28, 28, 28, 24, 30, 28, 28, 30, 28, 28,
            28, 28, 28, 28, 28, 28, 30, 28, 28, 28, 28, 28, 28, 28, 28, 28,
            6,  10, 10, 12, 13, 6,  8,  11, 10, 10, 8,  11, 8,  6,  6,  6,
            5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  7,  8,  15, 6,  12, 10,
            13, 6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
            7,  7,  7,  7,  7,  7,  7,  7,  8,  7,  8,  13, 19, 13, 14, 6,
            15, 5,  6,  5,  6,  5,  6,  6,  6,  5,  7,  7,  6,  6,  6,  5,
            6,  7,  6,  5,  5,  6,  7,  7,  7,  7,  7,  15, 11, 14, 13, 28,
            20, 22, 20, 20, 22, 22, 22, 23, 22, 23, 23, 23, 23, 23, 24, 23,
            24, 24, 22, 23, 24, 23, 23, 23, 23, 21, 22, 23, 22, 23, 23, 24,
            22, 21, 20, 22, 22, 23, 23, 21, 23, 22, 22, 24, 21, 22, 23, 23,
            21, 21, 22, 21, 23, 22, 23, 23, 20, 22, 22, 22, 23, 22, 22, 23,
            26, 26, 20, 19, 22, 23, 22, 25, 26, 26, 26, 27, 27, 26, 24, 25,
            19, 21, 26, 27, 27, 26, 27, 24, 21, 21, 26, 26, 28, 27, 27, 27,
            20, 24, 20, 21, 22, 21, 21, 23, 22, 22, 25, 25, 24, 24, 26, 23,
            26, 27, 26, 26, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 26};
    _main();
    return 0;
}
std::tuple<int, moku::error> HuffmanDecode(Writer w, moku::slice<uint8_t> v)
{
    moku::defer _defer_;
    Buffer *buf{std::nullptr};

    buf = moku::type_assert<*bytes::Buffer>(bufPool.Get());
    buf->Reset();
    _defer_.Push(
            [=]() mutable { bufPool.Put(moku::make_iface<Buffer *>(buf)); });
    {
        moku::error err{};

        err = huffmanDecode(buf, 0, v);

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    return w.Write(buf->Bytes());
}
std::tuple<std::string, moku::error> HuffmanDecodeToString(
        moku::slice<uint8_t> v)
{
    moku::defer _defer_;
    Buffer *buf{std::nullptr};

    buf = moku::type_assert<*bytes::Buffer>(bufPool.Get());
    buf->Reset();
    _defer_.Push(
            [=]() mutable { bufPool.Put(moku::make_iface<Buffer *>(buf)); });
    {
        moku::error err{};

        err = huffmanDecode(buf, 0, v);

        if (!moku::is_nil(err)) {
            return {"", err};
        }
    }
    return {buf->String(), nil};
}
moku::error huffmanDecode(Buffer *buf, int maxLen, moku::slice<uint8_t> v)
{
    uint8_t cbits{0};
    unsigned int cur{0};
    node *n{std::nullptr};
    uint8_t sbits{0};

    n = rootHuffmanNode;
    std::tie(cur, cbits, sbits) = std::tuple<unsigned int, uint8_t, uint8_t>(
            unsigned int(0), uint8_t(0), uint8_t(0));
    {
        uint8_t b{0};
        for (b : moku::range_value<moku::slice<uint8_t>>(v)) {
            cur = cur << 8 | unsigned int(b);
            cbits += 8;
            sbits += 8;
            while (cbits >= 8) {
                uint8_t idx{0};

                idx = byte(cur >> (cbits - 8));
                n = n->children[idx];
                if (moku::is_nil(n)) {
                    return ErrInvalidHuffman;
                }
                if (moku::is_nil(n->children)) {
                    if (maxLen != 0 && buf->Len() == maxLen) {
                        return ErrStringLength;
                    }
                    buf->WriteByte(n->sym);
                    cbits -= n->codeLen;
                    n = rootHuffmanNode;
                    sbits = cbits;
                } else {
                    cbits -= 8;
                }
            }
        }
    }
    while (cbits > 0) {
        n = n->children[byte(cur << (8 - cbits))];
        if (moku::is_nil(n)) {
            return ErrInvalidHuffman;
        }
        if (!moku::is_nil(n->children) || n->codeLen > cbits) {
            break;
        }
        if (maxLen != 0 && buf->Len() == maxLen) {
            return ErrStringLength;
        }
        buf->WriteByte(n->sym);
        cbits -= n->codeLen;
        n = rootHuffmanNode;
        sbits = cbits;
    }
    if (sbits > 7) {
        return ErrInvalidHuffman;
    }
    {
        unsigned int mask{0};

        mask = unsigned int(1 << cbits - 1);

        if (cur & mask != mask) {
            return ErrInvalidHuffman;
        }
    }
    return nil;
}
node *newInternalNode()
{
    return &node{{children, make(moku::slice<*node>, 256)}};
}
void init()
{
    if (len(huffmanCodes) != 256) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 5005736) STRING "unexpected size"}));
    }
    {
        uint32_t code{0};
        int i{0};
        for (std::tie(i, code) :
             moku::range_key_value<std::vector<uint32_t>>(huffmanCodes)) {
            addDecoderNode(byte(i), code, huffmanCodeLen[i]);
        }
    }
}
void addDecoderNode(uint8_t sym, uint32_t code, uint8_t codeLen)
{
    node *cur{std::nullptr};
    int end{0};
    uint8_t shift{0};
    int start{0};

    cur = rootHuffmanNode;
    while (codeLen > 8) {
        uint8_t i{0};

        codeLen -= 8;
        i = uint8_t(code >> codeLen);
        if (moku::is_nil(cur->children[i])) {
            cur->children[i] = newInternalNode();
        }
        cur = cur->children[i];
    }
    shift = 8 - codeLen;
    std::tie(start, end) =
            std::tuple<int, int>(int(uint8_t(code << shift)), int(1 << shift));
    {
        int i{0};
        for (i = start; i < start + end; i++) {
            cur->children[i] = &node{{sym, sym}, {codeLen, codeLen}};
        }
    }
}
moku::slice<uint8_t> AppendHuffmanString(moku::slice<uint8_t> dst,
                                         std::string s)
{
    uint8_t rembits{0};

    rembits = uint8_t(8);
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            if (rembits == 8) {
                dst = append(dst, 0);
            }
            std::tie(dst, rembits) =
                    appendByteToHuffmanCode(dst, rembits, s[i]);
        }
    }
    if (rembits < 8) {
        code = uint32_t(0x3fffffff);
        nbits = uint8_t(30);
        t = uint8_t(code >> (nbits - rembits));
        dst[len(dst) - 1] |= t;
    }
    return dst;
}
uint64_t HuffmanEncodeLength(std::string s)
{
    uint64_t n{0};

    n = uint64_t(0);
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            n += uint64_t(huffmanCodeLen[s[i]]);
        }
    }
    return (n + 7) / 8;
}
std::tuple<moku::slice<uint8_t>, uint8_t> appendByteToHuffmanCode(
        moku::slice<uint8_t> dst, uint8_t rembits, uint8_t c)
{
    uint32_t code{0};
    uint8_t nbits{0};

    code = huffmanCodes[c];
    nbits = huffmanCodeLen[c];
    while (true) {
        uint8_t t{0};

        if (rembits > nbits) {
            t = uint8_t(code << (rembits - nbits));
            dst[len(dst) - 1] |= t;
            rembits -= nbits;
            break;
        }
        t = uint8_t(code >> (nbits - rembits));
        dst[len(dst) - 1] |= t;
        nbits -= rembits;
        rembits = 8;
        if (nbits == 0) {
            break;
        }
        dst = append(dst, 0);
    }
    return {dst, rembits};
}
void init();
int main()
{
    init();
    ErrStringLength = errors::New("hpack: string too long");
    errNeedMore = errors::New("need more data");
    errVarintOverflow = DecodingError{errors::New("varint integer overflow")};
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidHuffman = errors::New("hpack: invalid Huffman-encoded data");
    rootHuffmanNode = newInternalNode();
    staticTableEntries = std::vector<HeaderField>{
            HeaderField{{Name, ":authority"}},
            HeaderField{{Name, ":method"}, {Value, "GET"}},
            HeaderField{{Name, ":method"}, {Value, "POST"}},
            HeaderField{{Name, ":path"}, {Value, "/"}},
            HeaderField{{Name, ":path"}, {Value, "/index.html"}},
            HeaderField{{Name, ":scheme"}, {Value, "http"}},
            HeaderField{{Name, ":scheme"}, {Value, "https"}},
            HeaderField{{Name, ":status"}, {Value, "200"}},
            HeaderField{{Name, ":status"}, {Value, "204"}},
            HeaderField{{Name, ":status"}, {Value, "206"}},
            HeaderField{{Name, ":status"}, {Value, "304"}},
            HeaderField{{Name, ":status"}, {Value, "400"}},
            HeaderField{{Name, ":status"}, {Value, "404"}},
            HeaderField{{Name, ":status"}, {Value, "500"}},
            HeaderField{{Name, "accept-charset"}},
            HeaderField{{Name, "accept-encoding"}, {Value, "gzip, deflate"}},
            HeaderField{{Name, "accept-language"}},
            HeaderField{{Name, "accept-ranges"}},
            HeaderField{{Name, "accept"}},
            HeaderField{{Name, "access-control-allow-origin"}},
            HeaderField{{Name, "age"}},
            HeaderField{{Name, "allow"}},
            HeaderField{{Name, "authorization"}},
            HeaderField{{Name, "cache-control"}},
            HeaderField{{Name, "content-disposition"}},
            HeaderField{{Name, "content-encoding"}},
            HeaderField{{Name, "content-language"}},
            HeaderField{{Name, "content-length"}},
            HeaderField{{Name, "content-location"}},
            HeaderField{{Name, "content-range"}},
            HeaderField{{Name, "content-type"}},
            HeaderField{{Name, "cookie"}},
            HeaderField{{Name, "date"}},
            HeaderField{{Name, "etag"}},
            HeaderField{{Name, "expect"}},
            HeaderField{{Name, "expires"}},
            HeaderField{{Name, "from"}},
            HeaderField{{Name, "host"}},
            HeaderField{{Name, "if-match"}},
            HeaderField{{Name, "if-modified-since"}},
            HeaderField{{Name, "if-none-match"}},
            HeaderField{{Name, "if-range"}},
            HeaderField{{Name, "if-unmodified-since"}},
            HeaderField{{Name, "last-modified"}},
            HeaderField{{Name, "link"}},
            HeaderField{{Name, "location"}},
            HeaderField{{Name, "max-forwards"}},
            HeaderField{{Name, "proxy-authenticate"}},
            HeaderField{{Name, "proxy-authorization"}},
            HeaderField{{Name, "range"}},
            HeaderField{{Name, "referer"}},
            HeaderField{{Name, "refresh"}},
            HeaderField{{Name, "retry-after"}},
            HeaderField{{Name, "server"}},
            HeaderField{{Name, "set-cookie"}},
            HeaderField{{Name, "strict-transport-security"}},
            HeaderField{{Name, "transfer-encoding"}},
            HeaderField{{Name, "user-agent"}},
            HeaderField{{Name, "vary"}},
            HeaderField{{Name, "via"}},
            HeaderField{{Name, "www-authenticate"}}};
    staticTable = newStaticTable();
    huffmanCodes = std::vector<uint32_t>{
            0x1ff8,    0x7fffd8,   0xfffffe2, 0xfffffe3, 0xfffffe4,  0xfffffe5,
            0xfffffe6, 0xfffffe7,  0xfffffe8, 0xffffea,  0x3ffffffc, 0xfffffe9,
            0xfffffea, 0x3ffffffd, 0xfffffeb, 0xfffffec, 0xfffffed,  0xfffffee,
            0xfffffef, 0xffffff0,  0xffffff1, 0xffffff2, 0x3ffffffe, 0xffffff3,
            0xffffff4, 0xffffff5,  0xffffff6, 0xffffff7, 0xffffff8,  0xffffff9,
            0xffffffa, 0xffffffb,  0x14,      0x3f8,     0x3f9,      0xffa,
            0x1ff9,    0x15,       0xf8,      0x7fa,     0x3fa,      0x3fb,
            0xf9,      0x7fb,      0xfa,      0x16,      0x17,       0x18,
            0x0,       0x1,        0x2,       0x19,      0x1a,       0x1b,
            0x1c,      0x1d,       0x1e,      0x1f,      0x5c,       0xfb,
            0x7ffc,    0x20,       0xffb,     0x3fc,     0x1ffa,     0x21,
            0x5d,      0x5e,       0x5f,      0x60,      0x61,       0x62,
            0x63,      0x64,       0x65,      0x66,      0x67,       0x68,
            0x69,      0x6a,       0x6b,      0x6c,      0x6d,       0x6e,
            0x6f,      0x70,       0x71,      0x72,      0xfc,       0x73,
            0xfd,      0x1ffb,     0x7fff0,   0x1ffc,    0x3ffc,     0x22,
            0x7ffd,    0x3,        0x23,      0x4,       0x24,       0x5,
            0x25,      0x26,       0x27,      0x6,       0x74,       0x75,
            0x28,      0x29,       0x2a,      0x7,       0x2b,       0x76,
            0x2c,      0x8,        0x9,       0x2d,      0x77,       0x78,
            0x79,      0x7a,       0x7b,      0x7ffe,    0x7fc,      0x3ffd,
            0x1ffd,    0xffffffc,  0xfffe6,   0x3fffd2,  0xfffe7,    0xfffe8,
            0x3fffd3,  0x3fffd4,   0x3fffd5,  0x7fffd9,  0x3fffd6,   0x7fffda,
            0x7fffdb,  0x7fffdc,   0x7fffdd,  0x7fffde,  0xffffeb,   0x7fffdf,
            0xffffec,  0xffffed,   0x3fffd7,  0x7fffe0,  0xffffee,   0x7fffe1,
            0x7fffe2,  0x7fffe3,   0x7fffe4,  0x1fffdc,  0x3fffd8,   0x7fffe5,
            0x3fffd9,  0x7fffe6,   0x7fffe7,  0xffffef,  0x3fffda,   0x1fffdd,
            0xfffe9,   0x3fffdb,   0x3fffdc,  0x7fffe8,  0x7fffe9,   0x1fffde,
            0x7fffea,  0x3fffdd,   0x3fffde,  0xfffff0,  0x1fffdf,   0x3fffdf,
            0x7fffeb,  0x7fffec,   0x1fffe0,  0x1fffe1,  0x3fffe0,   0x1fffe2,
            0x7fffed,  0x3fffe1,   0x7fffee,  0x7fffef,  0xfffea,    0x3fffe2,
            0x3fffe3,  0x3fffe4,   0x7ffff0,  0x3fffe5,  0x3fffe6,   0x7ffff1,
            0x3ffffe0, 0x3ffffe1,  0xfffeb,   0x7fff1,   0x3fffe7,   0x7ffff2,
            0x3fffe8,  0x1ffffec,  0x3ffffe2, 0x3ffffe3, 0x3ffffe4,  0x7ffffde,
            0x7ffffdf, 0x3ffffe5,  0xfffff1,  0x1ffffed, 0x7fff2,    0x1fffe3,
            0x3ffffe6, 0x7ffffe0,  0x7ffffe1, 0x3ffffe7, 0x7ffffe2,  0xfffff2,
            0x1fffe4,  0x1fffe5,   0x3ffffe8, 0x3ffffe9, 0xffffffd,  0x7ffffe3,
            0x7ffffe4, 0x7ffffe5,  0xfffec,   0xfffff3,  0xfffed,    0x1fffe6,
            0x3fffe9,  0x1fffe7,   0x1fffe8,  0x7ffff3,  0x3fffea,   0x3fffeb,
            0x1ffffee, 0x1ffffef,  0xfffff4,  0xfffff5,  0x3ffffea,  0x7ffff4,
            0x3ffffeb, 0x7ffffe6,  0x3ffffec, 0x3ffffed, 0x7ffffe7,  0x7ffffe8,
            0x7ffffe9, 0x7ffffea,  0x7ffffeb, 0xffffffe, 0x7ffffec,  0x7ffffed,
            0x7ffffee, 0x7ffffef,  0x7fffff0, 0x3ffffee};
    huffmanCodeLen = std::vector<uint8_t>{
            13, 23, 28, 28, 28, 28, 28, 28, 28, 24, 30, 28, 28, 30, 28, 28,
            28, 28, 28, 28, 28, 28, 30, 28, 28, 28, 28, 28, 28, 28, 28, 28,
            6,  10, 10, 12, 13, 6,  8,  11, 10, 10, 8,  11, 8,  6,  6,  6,
            5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  7,  8,  15, 6,  12, 10,
            13, 6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
            7,  7,  7,  7,  7,  7,  7,  7,  8,  7,  8,  13, 19, 13, 14, 6,
            15, 5,  6,  5,  6,  5,  6,  6,  6,  5,  7,  7,  6,  6,  6,  5,
            6,  7,  6,  5,  5,  6,  7,  7,  7,  7,  7,  15, 11, 14, 13, 28,
            20, 22, 20, 20, 22, 22, 22, 23, 22, 23, 23, 23, 23, 23, 24, 23,
            24, 24, 22, 23, 24, 23, 23, 23, 23, 21, 22, 23, 22, 23, 23, 24,
            22, 21, 20, 22, 22, 23, 23, 21, 23, 22, 22, 24, 21, 22, 23, 23,
            21, 21, 22, 21, 23, 22, 23, 23, 20, 22, 22, 22, 23, 22, 22, 23,
            26, 26, 20, 19, 22, 23, 22, 25, 26, 26, 26, 27, 27, 26, 24, 25,
            19, 21, 26, 27, 27, 26, 27, 24, 21, 21, 26, 26, 28, 27, 27, 27,
            20, 24, 20, 21, 22, 21, 21, 23, 22, 22, 25, 25, 24, 24, 26, 23,
            26, 27, 26, 26, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 26};
    _main();
    return 0;
}
void headerFieldTable::init()
{
    this->byName = make(std::map<std::string, uint64_t>);
    this->byNameValue = make(std::map<pairNameValue, uint64_t>);
}
int headerFieldTable::len()
{
    return len(this->ents);
}
void headerFieldTable::addEntry(HeaderField f)
{
    uint64_t id{0};

    id = uint64_t(this->len()) + this->evictCount + 1;
    this->byName[f.Name] = id;
    this->byNameValue[pairNameValue{f.Name, f.Value}] = id;
    this->ents = append(this->ents, f);
}
void headerFieldTable::evictOldest(int n)
{
    if (n > this->len()) {
        panic(moku::make_iface<std::string>(
                &{ % !s(*ast.SelectorExpr = &{0xc426b9bda0 0xc426b9bdc0}) %
                   !s(token.Pos = 4932297)
                           [% !s(*ast.BasicLit =
                                         &{4932298 9 "evictOldest(%v) on table "
                                                     "with %v entries"}) n %
                            !s(*ast.CallExpr =
                                       &{0xc426b9bea0 4932350 [] 0 4932351})] %
                   !s(token.Pos = 0) % !s(token.Pos = 4932352)}));
    }
    {
        int k{0};
        for (k = 0; k < n; k++) {
            HeaderField f{};
            uint64_t id{0};

            f = this->ents[k];
            id = this->evictCount + uint64_t(k) + 1;
            if (this->byName[f.Name] == id) {
                delete (this->byName, f.Name);
            }
            {
                pairNameValue p{};

                p = (pairNameValue{f.Name, f.Value});

                if (this->byNameValue[p] == id) {
                    delete (this->byNameValue, p);
                }
            }
        }
    }
    copy(this->ents, moku::slice_expr<moku::slice<HeaderField>>(this->ents, n));
    {
        int k{0};
        for (k = this->len() - n; k < this->len(); k++) {
            this->ents[k] = HeaderField{};
        }
    }
    this->ents = moku::slice_expr<moku::slice<HeaderField>>(this->ents,
                                                            this->len() - n);
    if (this->evictCount + uint64_t(n) < this->evictCount) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 4932823) STRING "evictCount overflow"}));
    }
    this->evictCount += uint64_t(n);
}
std::tuple<uint64_t, bool> headerFieldTable::search(HeaderField f)
{
    uint64_t i{0};
    bool nameValueMatch{false};

    if (!f.Sensitive) {
        {
            uint64_t id{0};

            id = this->byNameValue[pairNameValue{f.Name, f.Value}];

            if (id != 0) {
                return {this->idToIndex(id), true};
            }
        }
    }
    {
        uint64_t id{0};

        id = this->byName[f.Name];

        if (id != 0) {
            return {this->idToIndex(id), false};
        }
    }
    return {0, false};
}
uint64_t headerFieldTable::idToIndex(uint64_t id)
{
    uint64_t k{0};

    if (id <= this->evictCount) {
        panic(moku::make_iface<std::string>(&{
                % !s(*ast.SelectorExpr = &{0xc426c154c0 0xc426c154e0}) %
                !s(token.Pos = 4934031)
                        [% !s(*ast.BasicLit =
                                      &{4934032 9 "id (%v) <= evictCount (%v)"})
                                 id %
                         !s(*ast.SelectorExpr = &{0xc426c15580 0xc426c155a0})] %
                !s(token.Pos = 0) % !s(token.Pos = 4934078)}));
    }
    k = id - this->evictCount - 1;
    if (this != staticTable) {
        return uint64_t(this->len()) - k;
    }
    return k + 1;
}
headerFieldTable *newStaticTable()
{
    headerFieldTable *t{std::nullptr};

    t = &headerFieldTable{};
    t->init();
    {
        HeaderField e{};
        for (e : moku::range_value<moku::slice<HeaderField>>(
                     moku::slice_expr<std::vector<HeaderField>>(
                             staticTableEntries))) {
            t->addEntry(e);
        }
    }
    return t;
}
void init();
int main()
{
    init();
    ErrStringLength = errors::New("hpack: string too long");
    errNeedMore = errors::New("need more data");
    errVarintOverflow = DecodingError{errors::New("varint integer overflow")};
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidHuffman = errors::New("hpack: invalid Huffman-encoded data");
    rootHuffmanNode = newInternalNode();
    staticTableEntries = std::vector<HeaderField>{
            HeaderField{{Name, ":authority"}},
            HeaderField{{Name, ":method"}, {Value, "GET"}},
            HeaderField{{Name, ":method"}, {Value, "POST"}},
            HeaderField{{Name, ":path"}, {Value, "/"}},
            HeaderField{{Name, ":path"}, {Value, "/index.html"}},
            HeaderField{{Name, ":scheme"}, {Value, "http"}},
            HeaderField{{Name, ":scheme"}, {Value, "https"}},
            HeaderField{{Name, ":status"}, {Value, "200"}},
            HeaderField{{Name, ":status"}, {Value, "204"}},
            HeaderField{{Name, ":status"}, {Value, "206"}},
            HeaderField{{Name, ":status"}, {Value, "304"}},
            HeaderField{{Name, ":status"}, {Value, "400"}},
            HeaderField{{Name, ":status"}, {Value, "404"}},
            HeaderField{{Name, ":status"}, {Value, "500"}},
            HeaderField{{Name, "accept-charset"}},
            HeaderField{{Name, "accept-encoding"}, {Value, "gzip, deflate"}},
            HeaderField{{Name, "accept-language"}},
            HeaderField{{Name, "accept-ranges"}},
            HeaderField{{Name, "accept"}},
            HeaderField{{Name, "access-control-allow-origin"}},
            HeaderField{{Name, "age"}},
            HeaderField{{Name, "allow"}},
            HeaderField{{Name, "authorization"}},
            HeaderField{{Name, "cache-control"}},
            HeaderField{{Name, "content-disposition"}},
            HeaderField{{Name, "content-encoding"}},
            HeaderField{{Name, "content-language"}},
            HeaderField{{Name, "content-length"}},
            HeaderField{{Name, "content-location"}},
            HeaderField{{Name, "content-range"}},
            HeaderField{{Name, "content-type"}},
            HeaderField{{Name, "cookie"}},
            HeaderField{{Name, "date"}},
            HeaderField{{Name, "etag"}},
            HeaderField{{Name, "expect"}},
            HeaderField{{Name, "expires"}},
            HeaderField{{Name, "from"}},
            HeaderField{{Name, "host"}},
            HeaderField{{Name, "if-match"}},
            HeaderField{{Name, "if-modified-since"}},
            HeaderField{{Name, "if-none-match"}},
            HeaderField{{Name, "if-range"}},
            HeaderField{{Name, "if-unmodified-since"}},
            HeaderField{{Name, "last-modified"}},
            HeaderField{{Name, "link"}},
            HeaderField{{Name, "location"}},
            HeaderField{{Name, "max-forwards"}},
            HeaderField{{Name, "proxy-authenticate"}},
            HeaderField{{Name, "proxy-authorization"}},
            HeaderField{{Name, "range"}},
            HeaderField{{Name, "referer"}},
            HeaderField{{Name, "refresh"}},
            HeaderField{{Name, "retry-after"}},
            HeaderField{{Name, "server"}},
            HeaderField{{Name, "set-cookie"}},
            HeaderField{{Name, "strict-transport-security"}},
            HeaderField{{Name, "transfer-encoding"}},
            HeaderField{{Name, "user-agent"}},
            HeaderField{{Name, "vary"}},
            HeaderField{{Name, "via"}},
            HeaderField{{Name, "www-authenticate"}}};
    staticTable = newStaticTable();
    huffmanCodes = std::vector<uint32_t>{
            0x1ff8,    0x7fffd8,   0xfffffe2, 0xfffffe3, 0xfffffe4,  0xfffffe5,
            0xfffffe6, 0xfffffe7,  0xfffffe8, 0xffffea,  0x3ffffffc, 0xfffffe9,
            0xfffffea, 0x3ffffffd, 0xfffffeb, 0xfffffec, 0xfffffed,  0xfffffee,
            0xfffffef, 0xffffff0,  0xffffff1, 0xffffff2, 0x3ffffffe, 0xffffff3,
            0xffffff4, 0xffffff5,  0xffffff6, 0xffffff7, 0xffffff8,  0xffffff9,
            0xffffffa, 0xffffffb,  0x14,      0x3f8,     0x3f9,      0xffa,
            0x1ff9,    0x15,       0xf8,      0x7fa,     0x3fa,      0x3fb,
            0xf9,      0x7fb,      0xfa,      0x16,      0x17,       0x18,
            0x0,       0x1,        0x2,       0x19,      0x1a,       0x1b,
            0x1c,      0x1d,       0x1e,      0x1f,      0x5c,       0xfb,
            0x7ffc,    0x20,       0xffb,     0x3fc,     0x1ffa,     0x21,
            0x5d,      0x5e,       0x5f,      0x60,      0x61,       0x62,
            0x63,      0x64,       0x65,      0x66,      0x67,       0x68,
            0x69,      0x6a,       0x6b,      0x6c,      0x6d,       0x6e,
            0x6f,      0x70,       0x71,      0x72,      0xfc,       0x73,
            0xfd,      0x1ffb,     0x7fff0,   0x1ffc,    0x3ffc,     0x22,
            0x7ffd,    0x3,        0x23,      0x4,       0x24,       0x5,
            0x25,      0x26,       0x27,      0x6,       0x74,       0x75,
            0x28,      0x29,       0x2a,      0x7,       0x2b,       0x76,
            0x2c,      0x8,        0x9,       0x2d,      0x77,       0x78,
            0x79,      0x7a,       0x7b,      0x7ffe,    0x7fc,      0x3ffd,
            0x1ffd,    0xffffffc,  0xfffe6,   0x3fffd2,  0xfffe7,    0xfffe8,
            0x3fffd3,  0x3fffd4,   0x3fffd5,  0x7fffd9,  0x3fffd6,   0x7fffda,
            0x7fffdb,  0x7fffdc,   0x7fffdd,  0x7fffde,  0xffffeb,   0x7fffdf,
            0xffffec,  0xffffed,   0x3fffd7,  0x7fffe0,  0xffffee,   0x7fffe1,
            0x7fffe2,  0x7fffe3,   0x7fffe4,  0x1fffdc,  0x3fffd8,   0x7fffe5,
            0x3fffd9,  0x7fffe6,   0x7fffe7,  0xffffef,  0x3fffda,   0x1fffdd,
            0xfffe9,   0x3fffdb,   0x3fffdc,  0x7fffe8,  0x7fffe9,   0x1fffde,
            0x7fffea,  0x3fffdd,   0x3fffde,  0xfffff0,  0x1fffdf,   0x3fffdf,
            0x7fffeb,  0x7fffec,   0x1fffe0,  0x1fffe1,  0x3fffe0,   0x1fffe2,
            0x7fffed,  0x3fffe1,   0x7fffee,  0x7fffef,  0xfffea,    0x3fffe2,
            0x3fffe3,  0x3fffe4,   0x7ffff0,  0x3fffe5,  0x3fffe6,   0x7ffff1,
            0x3ffffe0, 0x3ffffe1,  0xfffeb,   0x7fff1,   0x3fffe7,   0x7ffff2,
            0x3fffe8,  0x1ffffec,  0x3ffffe2, 0x3ffffe3, 0x3ffffe4,  0x7ffffde,
            0x7ffffdf, 0x3ffffe5,  0xfffff1,  0x1ffffed, 0x7fff2,    0x1fffe3,
            0x3ffffe6, 0x7ffffe0,  0x7ffffe1, 0x3ffffe7, 0x7ffffe2,  0xfffff2,
            0x1fffe4,  0x1fffe5,   0x3ffffe8, 0x3ffffe9, 0xffffffd,  0x7ffffe3,
            0x7ffffe4, 0x7ffffe5,  0xfffec,   0xfffff3,  0xfffed,    0x1fffe6,
            0x3fffe9,  0x1fffe7,   0x1fffe8,  0x7ffff3,  0x3fffea,   0x3fffeb,
            0x1ffffee, 0x1ffffef,  0xfffff4,  0xfffff5,  0x3ffffea,  0x7ffff4,
            0x3ffffeb, 0x7ffffe6,  0x3ffffec, 0x3ffffed, 0x7ffffe7,  0x7ffffe8,
            0x7ffffe9, 0x7ffffea,  0x7ffffeb, 0xffffffe, 0x7ffffec,  0x7ffffed,
            0x7ffffee, 0x7ffffef,  0x7fffff0, 0x3ffffee};
    huffmanCodeLen = std::vector<uint8_t>{
            13, 23, 28, 28, 28, 28, 28, 28, 28, 24, 30, 28, 28, 30, 28, 28,
            28, 28, 28, 28, 28, 28, 30, 28, 28, 28, 28, 28, 28, 28, 28, 28,
            6,  10, 10, 12, 13, 6,  8,  11, 10, 10, 8,  11, 8,  6,  6,  6,
            5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  7,  8,  15, 6,  12, 10,
            13, 6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
            7,  7,  7,  7,  7,  7,  7,  7,  8,  7,  8,  13, 19, 13, 14, 6,
            15, 5,  6,  5,  6,  5,  6,  6,  6,  5,  7,  7,  6,  6,  6,  5,
            6,  7,  6,  5,  5,  6,  7,  7,  7,  7,  7,  15, 11, 14, 13, 28,
            20, 22, 20, 20, 22, 22, 22, 23, 22, 23, 23, 23, 23, 23, 24, 23,
            24, 24, 22, 23, 24, 23, 23, 23, 23, 21, 22, 23, 22, 23, 23, 24,
            22, 21, 20, 22, 22, 23, 23, 21, 23, 22, 22, 24, 21, 22, 23, 23,
            21, 21, 22, 21, 23, 22, 23, 23, 20, 22, 22, 22, 23, 22, 22, 23,
            26, 26, 20, 19, 22, 23, 22, 25, 26, 26, 26, 27, 27, 26, 24, 25,
            19, 21, 26, 27, 27, 26, 27, 24, 21, 21, 26, 26, 28, 27, 27, 27,
            20, 24, 20, 21, 22, 21, 21, 23, 22, 22, 25, 25, 24, 24, 26, 23,
            26, 27, 26, 26, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 26};
    _main();
    return 0;
}
