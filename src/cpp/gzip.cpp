moku::error noEOF(moku::error err)
{
    if (err == io::EOF) {
        return io::ErrUnexpectedEOF;
    }
    return err;
}
std::tuple<Reader *, moku::error> NewReader(Reader r)
{
    Reader *z{std::nullptr};

    z = new (Reader);
    {
        moku::error err{};

        err = z->Reset(r);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    return {z, nil};
}
moku::error Reader::Reset(Reader r)
{
    *this = Reader{decompressor : this->decompressor, multistream : true};
    {
        bool ok{false};
        Reader rr{};

        std::tie(rr, ok) = moku::try_assert<flate::Reader>(r);

        if (ok) {
            this->r = rr;
        } else {
            this->r = bufio::NewReader(r);
        }
    }
    std::tie(this->Header, this->err) = this->readHeader();
    return this->err;
}
void Reader::Multistream(bool ok)
{
    this->multistream = ok;
}
std::tuple<std::string, moku::error> Reader::readString()
{
    moku::error err{};
    bool needConv{false};

    needConv = false;
    {
        int i{0};
        for (i = 0;; i++) {
            if (i >= len(this->buf)) {
                return {"", ErrHeader};
            }
            std::tie(this->buf[i], err) = this->r.ReadByte();
            if (!moku::is_nil(err)) {
                return {"", err};
            }
            if (this->buf[i] > 0x7f) {
                needConv = true;
            }
            if (this->buf[i] == 0) {
                this->digest =
                        crc32::Update(this->digest,
                                      crc32::IEEETable,
                                      moku::slice_expr<std::vector<uint8_t>>(
                                              this->buf, i + 1));
                if (needConv) {
                    s = make(moku::slice<rune>, 0, i);
                    {
                        uint8_t v{0};
                        for (v : moku::range_value<moku::slice<uint8_t>>(
                                     moku::slice_expr<std::vector<uint8_t>>(
                                             this->buf, i))) {
                            s = append(s, rune(v));
                        }
                    }
                    return {std::string(s), nil};
                }
                return {std::string(moku::slice_expr<std::vector<uint8_t>>(
                                this->buf, i)),
                        nil};
            }
        }
    }
}
std::tuple<Header, moku::error> Reader::readHeader()
{
    moku::error err{};
    uint8_t flg{0};
    Header hdr{};
    std::string s{""};

    {
        std::tie(_, err) = io::ReadFull(
                this->r, moku::slice_expr<std::vector<uint8_t>>(this->buf, 10));

        if (!moku::is_nil(err)) {
            return {hdr, err};
        }
    }
    if (this->buf[0] != gzipID1 || this->buf[1] != gzipID2 ||
        this->buf[2] != gzipDeflate) {
        return {hdr, ErrHeader};
    }
    flg = this->buf[3];
    {
        int64_t t{0};

        t = int64_t(le.Uint32(
                moku::slice_expr<std::vector<uint8_t>>(this->buf, 4, 8)));

        if (t > 0) {
            hdr.ModTime = time::Unix(t, 0);
        }
    }
    hdr.OS = this->buf[9];
    this->digest = crc32::ChecksumIEEE(
            moku::slice_expr<std::vector<uint8_t>>(this->buf, 10));
    if (flg & flagExtra != 0) {
        {
            std::tie(_, err) = io::ReadFull(
                    this->r,
                    moku::slice_expr<std::vector<uint8_t>>(this->buf, 2));

            if (!moku::is_nil(err)) {
                return {hdr, noEOF(err)};
            }
        }
        this->digest = crc32::Update(
                this->digest,
                crc32::IEEETable,
                moku::slice_expr<std::vector<uint8_t>>(this->buf, 2));
        data = make(moku::slice<byte>,
                    le.Uint16(moku::slice_expr<std::vector<uint8_t>>(this->buf,
                                                                     2)));
        {
            std::tie(_, err) = io::ReadFull(this->r, data);

            if (!moku::is_nil(err)) {
                return {hdr, noEOF(err)};
            }
        }
        this->digest = crc32::Update(this->digest, crc32::IEEETable, data);
        hdr.Extra = data;
    }
    if (flg & flagName != 0) {
        {
            std::tie(s, err) = this->readString();

            if (!moku::is_nil(err)) {
                return {hdr, err};
            }
        }
        hdr.Name = s;
    }
    if (flg & flagComment != 0) {
        {
            std::tie(s, err) = this->readString();

            if (!moku::is_nil(err)) {
                return {hdr, err};
            }
        }
        hdr.Comment = s;
    }
    if (flg & flagHdrCrc != 0) {
        {
            std::tie(_, err) = io::ReadFull(
                    this->r,
                    moku::slice_expr<std::vector<uint8_t>>(this->buf, 2));

            if (!moku::is_nil(err)) {
                return {hdr, noEOF(err)};
            }
        }
        digest =
                le.Uint16(moku::slice_expr<std::vector<uint8_t>>(this->buf, 2));
        if (digest != uint16_t(this->digest)) {
            return {hdr, ErrHeader};
        }
    }
    this->digest = 0;
    if (moku::is_nil(this->decompressor)) {
        this->decompressor = flate::NewReader(this->r);
    } else {
        moku::type_assert<flate::Resetter>(this->decompressor)
                .Reset(this->r, nil);
    }
    return {hdr, nil};
}
std::tuple<int, moku::error> Reader::Read(moku::slice<uint8_t> p)
{
    uint32_t digest{0};
    moku::error err{};
    int n{0};
    uint32_t size{0};

    if (!moku::is_nil(this->err)) {
        return {0, this->err};
    }
    std::tie(n, this->err) = this->decompressor.Read(p);
    this->digest = crc32::Update(this->digest,
                                 crc32::IEEETable,
                                 moku::slice_expr<moku::slice<uint8_t>>(p, n));
    this->size += uint32_t(n);
    if (this->err != io::EOF) {
        return {n, this->err};
    }
    {
        moku::error err{};

        std::tie(_, err) = io::ReadFull(
                this->r, moku::slice_expr<std::vector<uint8_t>>(this->buf, 8));

        if (!moku::is_nil(err)) {
            this->err = noEOF(err);
            return {n, this->err};
        }
    }
    digest = le.Uint32(moku::slice_expr<std::vector<uint8_t>>(this->buf, 4));
    size = le.Uint32(moku::slice_expr<std::vector<uint8_t>>(this->buf, 4, 8));
    if (digest != this->digest || size != this->size) {
        this->err = ErrChecksum;
        return {n, this->err};
    }
    std::tie(this->digest, this->size) = std::tuple<uint32_t, uint32_t>(0, 0);
    if (!this->multistream) {
        return {n, io::EOF};
    }
    this->err = nil;
    {
        std::tie(_, this->err) = this->readHeader();

        if (!moku::is_nil(this->err)) {
            return {n, this->err};
        }
    }
    if (n > 0) {
        return {n, nil};
    }
    return this->Read(p);
}
moku::error Reader::Close()
{
    return this->decompressor.Close();
}
void init();
int main()
{
    init();
    ErrChecksum = errors::New("gzip: invalid checksum");
    ErrHeader = errors::New("gzip: invalid header");
    le = binary::LittleEndian;
    _main();
    return 0;
}
Writer *NewWriter(Writer w)
{
    Writer *z{std::nullptr};

    std::tie(z, _) = NewWriterLevel(w, DefaultCompression);
    return z;
}
std::tuple<Writer *, moku::error> NewWriterLevel(Writer w, int level)
{
    Writer *z{std::nullptr};

    if (level < HuffmanOnly || level > BestCompression) {
        return {nil, fmt::Errorf("gzip: invalid compression level: %d", level)};
    }
    z = new (Writer);
    z->init(w, level);
    return {z, nil};
}
void Writer::init(Writer w, int level)
{
    Writer *compressor{std::nullptr};

    compressor = this->compressor;
    if (!moku::is_nil(compressor)) {
        compressor->Reset(w);
    }
    *this = Writer{
        Header : Header{OS : 255},
        w : w,
        level : level,
        compressor : compressor
    };
}
void Writer::Reset(Writer w)
{
    this->init(w, this->level);
}
moku::error Writer::writeBytes(moku::slice<uint8_t> b)
{
    moku::error err{};

    if (len(b) > 0xffff) {
        return errors::New("gzip.Write: Extra data is too large");
    }
    le.PutUint16(moku::slice_expr<std::vector<uint8_t>>(this->buf, 2),
                 uint16_t(len(b)));
    std::tie(_, err) =
            this->w.Write(moku::slice_expr<std::vector<uint8_t>>(this->buf, 2));
    if (!moku::is_nil(err)) {
        return err;
    }
    std::tie(_, err) = this->w.Write(b);
    return err;
}
moku::error Writer::writeString(std::string s)
{
    moku::error err{};
    bool needconv{false};

    needconv = false;
    {
        int32_t v{0};
        for (v : moku::range_value<std::string>(s)) {
            if (v == 0 || v > 0xff) {
                return errors::New("gzip.Write: non-Latin-1 header string");
            }
            if (v > 0x7f) {
                needconv = true;
            }
        }
    }
    if (needconv) {
        b = make(moku::slice<byte>, 0, len(s));
        {
            int32_t v{0};
            for (v : moku::range_value<std::string>(s)) {
                b = append(b, byte(v));
            }
        }
        std::tie(_, err) = this->w.Write(b);
    } else {
        std::tie(_, err) = io::WriteString(this->w, s);
    }
    if (!moku::is_nil(err)) {
        return err;
    }
    this->buf[0] = 0;
    std::tie(_, err) =
            this->w.Write(moku::slice_expr<std::vector<uint8_t>>(this->buf, 1));
    return err;
}
std::tuple<int, moku::error> Writer::Write(moku::slice<uint8_t> p)
{
    int n{0};

    if (!moku::is_nil(this->err)) {
        return {0, this->err};
    }
    if (!this->wroteHeader) {
        this->wroteHeader = true;
        this->buf =
                std::vector<byte>{{0, gzipID1}, {1, gzipID2}, {2, gzipDeflate}};
        if (!moku::is_nil(this->Extra)) {
            this->buf[3] |= 0x04;
        }
        if (this->Name != "") {
            this->buf[3] |= 0x08;
        }
        if (this->Comment != "") {
            this->buf[3] |= 0x10;
        }
        if (this->ModTime.After(time::Unix(0, 0))) {
            le.PutUint32(
                    moku::slice_expr<std::vector<uint8_t>>(this->buf, 4, 8),
                    uint32_t(this->ModTime.Unix()));
        }
        if (this->level == BestCompression) {
            this->buf[8] = 2;
        } else {
            if (this->level == BestSpeed) {
                this->buf[8] = 4;
            }
        }
        this->buf[9] = this->OS;
        std::tie(n, this->err) = this->w.Write(
                moku::slice_expr<std::vector<uint8_t>>(this->buf, 10));
        if (!moku::is_nil(this->err)) {
            return {n, this->err};
        }
        if (!moku::is_nil(this->Extra)) {
            this->err = this->writeBytes(this->Extra);
            if (!moku::is_nil(this->err)) {
                return {n, this->err};
            }
        }
        if (this->Name != "") {
            this->err = this->writeString(this->Name);
            if (!moku::is_nil(this->err)) {
                return {n, this->err};
            }
        }
        if (this->Comment != "") {
            this->err = this->writeString(this->Comment);
            if (!moku::is_nil(this->err)) {
                return {n, this->err};
            }
        }
        if (moku::is_nil(this->compressor)) {
            std::tie(this->compressor, _) =
                    flate::NewWriter(this->w, this->level);
        }
    }
    this->size += uint32_t(len(p));
    this->digest = crc32::Update(this->digest, crc32::IEEETable, p);
    std::tie(n, this->err) = this->compressor->Write(p);
    return {n, this->err};
}
moku::error Writer::Flush()
{
    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    if (this->closed) {
        return nil;
    }
    if (!this->wroteHeader) {
        this->Write(nil);
        if (!moku::is_nil(this->err)) {
            return this->err;
        }
    }
    this->err = this->compressor->Flush();
    return this->err;
}
moku::error Writer::Close()
{
    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    if (this->closed) {
        return nil;
    }
    this->closed = true;
    if (!this->wroteHeader) {
        this->Write(nil);
        if (!moku::is_nil(this->err)) {
            return this->err;
        }
    }
    this->err = this->compressor->Close();
    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    le.PutUint32(moku::slice_expr<std::vector<uint8_t>>(this->buf, 4),
                 this->digest);
    le.PutUint32(moku::slice_expr<std::vector<uint8_t>>(this->buf, 4, 8),
                 this->size);
    std::tie(_, this->err) =
            this->w.Write(moku::slice_expr<std::vector<uint8_t>>(this->buf, 8));
    return this->err;
}
void init();
int main()
{
    init();
    ErrChecksum = errors::New("gzip: invalid checksum");
    ErrHeader = errors::New("gzip: invalid header");
    le = binary::LittleEndian;
    _main();
    return 0;
}
