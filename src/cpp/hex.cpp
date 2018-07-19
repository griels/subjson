int EncodedLen(int n)
{
    return n * 2;
}
int Encode(moku::slice<uint8_t> dst, moku::slice<uint8_t> src)
{
    {
        int i{0};
        uint8_t v{0};
        for (std::tie(i, v) :
             moku::range_key_value<moku::slice<uint8_t>>(src)) {
            dst[i * 2] = hextable[v >> 4];
            dst[i * 2 + 1] = hextable[v & 0x0f];
        }
    }
    return len(src) * 2;
}
std::string InvalidByteError::_ErrorByValue()
{
    return fmt::Sprintf("encoding/hex: invalid byte: %#U", rune(this));
}
int DecodedLen(int x)
{
    return x / 2;
}
std::tuple<int, moku::error> Decode(moku::slice<uint8_t> dst,
                                    moku::slice<uint8_t> src)
{
    if (len(src) % 2 == 1) {
        return {0, ErrLength};
    }
    {
        int i{0};
        for (i = 0; i < len(src) / 2; i++) {
            uint8_t a{0};
            uint8_t b{0};
            bool ok{false};

            std::tie(a, ok) = fromHexChar(src[i * 2]);
            if (!ok) {
                return {0, InvalidByteError(src[i * 2])};
            }
            std::tie(b, ok) = fromHexChar(src[i * 2 + 1]);
            if (!ok) {
                return {0, InvalidByteError(src[i * 2 + 1])};
            }
            dst[i] = (a << 4) | b;
        }
    }
    return {len(src) / 2, nil};
}
std::tuple<uint8_t, bool> fromHexChar(uint8_t c)
{
    if (('0' <= c && c <= '9')) {
    _ident_0_:
        return {c - '0', true};
    } else if (('a' <= c && c <= 'f')) {
    _ident_1_:
        return {c - 'a' + 10, true};
    } else if (('A' <= c && c <= 'F')) {
    _ident_2_:
        return {c - 'A' + 10, true};
    }
    return {0, false};
}
std::string EncodeToString(moku::slice<uint8_t> src)
{
    moku::slice<uint8_t> dst{};

    dst = make(moku::slice<byte>, EncodedLen(len(src)));
    Encode(dst, src);
    return std::string(dst);
}
std::tuple<moku::slice<uint8_t>, moku::error> DecodeString(std::string s)
{
    moku::slice<uint8_t> dst{};
    moku::error err{};
    moku::slice<uint8_t> src{};

    src = moku::slice<byte>(s);
    dst = make(moku::slice<byte>, DecodedLen(len(src)));
    std::tie(_, err) = Decode(dst, src);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    return {dst, nil};
}
std::string Dump(moku::slice<uint8_t> data)
{
    Buffer buf{};
    WriteCloser dumper{};

    dumper = Dumper(&buf);
    dumper.Write(data);
    dumper.Close();
    return buf.String();
}
WriteCloser Dumper(Writer w)
{
    return &dumper{{w, w}};
}
uint8_t toChar(uint8_t b)
{
    if (b < 32 || b > 126) {
        return '.';
    }
    return b;
}
std::tuple<int, moku::error> dumper::Write(moku::slice<uint8_t> data)
{
    moku::error err{};
    int n{0};

    {
        int i{0};
        for (i : moku::range_key<moku::slice<uint8_t>>(data)) {
            if (this->used == 0) {
                this->buf[0] = byte(this->n >> 24);
                this->buf[1] = byte(this->n >> 16);
                this->buf[2] = byte(this->n >> 8);
                this->buf[3] = byte(this->n);
                Encode(moku::slice_expr<std::vector<uint8_t>>(this->buf, 4),
                       moku::slice_expr<std::vector<uint8_t>>(this->buf, 4));
                this->buf[12] = ' ';
                this->buf[13] = ' ';
                std::tie(_, err) = this->w.Write(
                        moku::slice_expr<std::vector<uint8_t>>(this->buf, 4));
                if (!moku::is_nil(err)) {
                    return;
                }
            }
            Encode(moku::slice_expr<std::vector<uint8_t>>(this->buf),
                   moku::slice_expr<moku::slice<uint8_t>>(data, i, i + 1));
            this->buf[2] = ' ';
            l = 3;
            if (this->used == 7) {
                this->buf[3] = ' ';
                l = 4;
            } else {
                if (this->used == 15) {
                    this->buf[3] = ' ';
                    this->buf[4] = '|';
                    l = 5;
                }
            }
            std::tie(_, err) = this->w.Write(
                    moku::slice_expr<std::vector<uint8_t>>(this->buf, l));
            if (!moku::is_nil(err)) {
                return;
            }
            n++;
            this->rightChars[this->used] = toChar(data[i]);
            this->used++;
            this->n++;
            if (this->used == 16) {
                this->rightChars[16] = '|';
                this->rightChars[17] = '\n';
                std::tie(_, err) =
                        this->w.Write(moku::slice_expr<std::vector<uint8_t>>(
                                this->rightChars));
                if (!moku::is_nil(err)) {
                    return;
                }
                this->used = 0;
            }
        }
    }
    return;
}
moku::error dumper::Close()
{
    moku::error err{};
    int nBytes{0};

    if (this->used == 0) {
        return;
    }
    this->buf[0] = ' ';
    this->buf[1] = ' ';
    this->buf[2] = ' ';
    this->buf[3] = ' ';
    this->buf[4] = '|';
    nBytes = this->used;
    while (this->used < 16) {
        int l{0};

        l = 3;
        if (this->used == 7) {
            l = 4;
        } else {
            if (this->used == 15) {
                l = 5;
            }
        }
        std::tie(_, err) = this->w.Write(
                moku::slice_expr<std::vector<uint8_t>>(this->buf, l));
        if (!moku::is_nil(err)) {
            return;
        }
        this->used++;
    }
    this->rightChars[nBytes] = '|';
    this->rightChars[nBytes + 1] = '\n';
    std::tie(_, err) = this->w.Write(moku::slice_expr<std::vector<uint8_t>>(
            this->rightChars, nBytes + 2));
    return;
}
int main()
{
    ErrLength = errors::New("encoding/hex: odd length hex string");
    _main();
    return 0;
}
