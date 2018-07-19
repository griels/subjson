moku::slice<uint8_t> Buffer::Bytes()
{
    return moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off);
}
std::string Buffer::String()
{
    if (moku::is_nil(this)) {
        return "<nil>";
    }
    return std::string(
            moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off));
}
int Buffer::Len()
{
    return len(this->buf) - this->off;
}
int Buffer::Cap()
{
    return cap(this->buf);
}
void Buffer::Truncate(int n)
{
    if (n == 0) {
        this->Reset();
        return;
    }
    this->lastRead = opInvalid;
    if (n < 0 || n > this->Len()) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 1498437) STRING
                     "bytes.Buffer: truncation out of range"}));
    }
    this->buf =
            moku::slice_expr<moku::slice<uint8_t>>(this->buf, 0, this->off + n);
}
void Buffer::Reset()
{
    this->buf = moku::slice_expr<moku::slice<uint8_t>>(this->buf, 0);
    this->off = 0;
    this->lastRead = opInvalid;
}
std::tuple<int, bool> Buffer::tryGrowByReslice(int n)
{
    {
        int l{0};

        l = len(this->buf);

        if (l + n <= cap(this->buf)) {
            this->buf =
                    moku::slice_expr<moku::slice<uint8_t>>(this->buf, l + n);
            return {l, true};
        }
    }
    return {0, false};
}
int Buffer::grow(int n)
{
    int m{0};

    m = this->Len();
    if (m == 0 && this->off != 0) {
        this->Reset();
    }
    {
        int i{0};
        bool ok{false};

        std::tie(i, ok) = this->tryGrowByReslice(n);

        if (ok) {
            return i;
        }
    }
    if (moku::is_nil(this->buf) && n <= len(this->bootstrap)) {
        this->buf = moku::slice_expr<std::vector<uint8_t>>(this->bootstrap, n);
        return 0;
    }
    if (m + n <= cap(this->buf) / 2) {
        copy(moku::slice_expr<moku::slice<uint8_t>>(this->buf),
             moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off));
    } else {
        buf = makeSlice(2 * cap(this->buf) + n);
        copy(buf, moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off));
        this->buf = buf;
    }
    this->off = 0;
    this->buf = moku::slice_expr<moku::slice<uint8_t>>(this->buf, m + n);
    return m;
}
void Buffer::Grow(int n)
{
    int m{0};

    if (n < 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 1500471) STRING
                     "bytes.Buffer.Grow: negative count"}));
    }
    m = this->grow(n);
    this->buf = moku::slice_expr<moku::slice<uint8_t>>(this->buf, 0, m);
}
std::tuple<int, moku::error> Buffer::Write(moku::slice<uint8_t> p)
{
    moku::error err{};
    int m{0};
    int n{0};
    bool ok{false};

    this->lastRead = opInvalid;
    std::tie(m, ok) = this->tryGrowByReslice(len(p));
    if (!ok) {
        m = this->grow(len(p));
    }
    return {copy(moku::slice_expr<moku::slice<uint8_t>>(this->buf, m), p), nil};
}
std::tuple<int, moku::error> Buffer::WriteString(std::string s)
{
    moku::error err{};
    int m{0};
    int n{0};
    bool ok{false};

    this->lastRead = opInvalid;
    std::tie(m, ok) = this->tryGrowByReslice(len(s));
    if (!ok) {
        m = this->grow(len(s));
    }
    return {copy(moku::slice_expr<moku::slice<uint8_t>>(this->buf, m), s), nil};
}
std::tuple<int64_t, moku::error> Buffer::ReadFrom(Reader r)
{
    moku::error err{};
    int64_t n{0};

    this->lastRead = opInvalid;
    if (this->off >= len(this->buf)) {
        this->Reset();
    }
    while (true) {
        moku::error e{};
        int m{0};

        {
            int free{0};

            free = cap(this->buf) - len(this->buf);

            if (free < MinRead) {
                newBuf = this->buf;
                if (this->off + free < MinRead) {
                    newBuf = makeSlice(2 * cap(this->buf) + MinRead);
                }
                copy(newBuf,
                     moku::slice_expr<moku::slice<uint8_t>>(this->buf,
                                                            this->off));
                this->buf = moku::slice_expr<moku::slice<uint8_t>>(
                        newBuf, len(this->buf) - this->off);
                this->off = 0;
            }
        }
        std::tie(m, e) = r.Read(moku::slice_expr<moku::slice<uint8_t>>(
                this->buf, len(this->buf), cap(this->buf)));
        this->buf = moku::slice_expr<moku::slice<uint8_t>>(
                this->buf, 0, len(this->buf) + m);
        n += int64_t(m);
        if (e == io::EOF) {
            break;
        }
        if (!moku::is_nil(e)) {
            return {n, e};
        }
    }
    return {n, nil};
}
moku::slice<uint8_t> makeSlice(int n)
{
    moku::defer _defer_;

    _defer_.Push([=]() mutable {
        [=]() mutable -> void {
            if (!moku::is_nil(recover())) {
                panic(moku::make_iface<moku::error>(ErrTooLarge));
            }
        }();
    });
    return make(moku::slice<byte>, n);
}
std::tuple<int64_t, moku::error> Buffer::WriteTo(Writer w)
{
    moku::error err{};
    int64_t n{0};

    this->lastRead = opInvalid;
    if (this->off < len(this->buf)) {
        nBytes = this->Len();
        std::tie(m, e) = w.Write(
                moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off));
        if (m > nBytes) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 1503404) STRING
                         "bytes.Buffer.WriteTo: invalid Write count"}));
        }
        this->off += m;
        n = int64_t(m);
        if (!moku::is_nil(e)) {
            return {n, e};
        }
        if (m != nBytes) {
            return {n, io::ErrShortWrite};
        }
    }
    this->Reset();
    return;
}
moku::error Buffer::WriteByte(uint8_t c)
{
    int m{0};
    bool ok{false};

    this->lastRead = opInvalid;
    std::tie(m, ok) = this->tryGrowByReslice(1);
    if (!ok) {
        m = this->grow(1);
    }
    this->buf[m] = c;
    return nil;
}
std::tuple<int, moku::error> Buffer::WriteRune(int32_t r)
{
    moku::error err{};
    int m{0};
    int n{0};
    bool ok{false};

    if (r < utf8::RuneSelf) {
        this->WriteByte(byte(r));
        return {1, nil};
    }
    this->lastRead = opInvalid;
    std::tie(m, ok) = this->tryGrowByReslice(utf8::UTFMax);
    if (!ok) {
        m = this->grow(utf8::UTFMax);
    }
    n = utf8::EncodeRune(moku::slice_expr<moku::slice<uint8_t>>(
                                 this->buf, m, m + utf8::UTFMax),
                         r);
    this->buf = moku::slice_expr<moku::slice<uint8_t>>(this->buf, m + n);
    return {n, nil};
}
std::tuple<int, moku::error> Buffer::Read(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    this->lastRead = opInvalid;
    if (this->off >= len(this->buf)) {
        this->Reset();
        if (len(p) == 0) {
            return;
        }
        return {0, io::EOF};
    }
    n = copy(p, moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off));
    this->off += n;
    if (n > 0) {
        this->lastRead = opRead;
    }
    return;
}
moku::slice<uint8_t> Buffer::Next(int n)
{
    moku::slice<uint8_t> data{};
    int m{0};

    this->lastRead = opInvalid;
    m = this->Len();
    if (n > m) {
        n = m;
    }
    data = moku::slice_expr<moku::slice<uint8_t>>(
            this->buf, this->off, this->off + n);
    this->off += n;
    if (n > 0) {
        this->lastRead = opRead;
    }
    return data;
}
std::tuple<uint8_t, moku::error> Buffer::ReadByte()
{
    uint8_t c{0};

    this->lastRead = opInvalid;
    if (this->off >= len(this->buf)) {
        this->Reset();
        return {0, io::EOF};
    }
    c = this->buf[this->off];
    this->off++;
    this->lastRead = opRead;
    return {c, nil};
}
std::tuple<int32_t, int, moku::error> Buffer::ReadRune()
{
    uint8_t c{0};
    moku::error err{};
    int n{0};
    int32_t r{0};
    int size{0};

    this->lastRead = opInvalid;
    if (this->off >= len(this->buf)) {
        this->Reset();
        return {0, 0, io::EOF};
    }
    c = this->buf[this->off];
    if (c < utf8::RuneSelf) {
        this->off++;
        this->lastRead = opReadRune1;
        return {rune(c), 1, nil};
    }
    std::tie(r, n) = utf8::DecodeRune(
            moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off));
    this->off += n;
    this->lastRead = readOp(n);
    return {r, n, nil};
}
moku::error Buffer::UnreadRune()
{
    if (this->lastRead <= opInvalid) {
        return errors::New(
                "bytes.Buffer: UnreadRune: previous operation was not a "
                "successful ReadRune");
    }
    if (this->off >= int(this->lastRead)) {
        this->off -= int(this->lastRead);
    }
    this->lastRead = opInvalid;
    return nil;
}
moku::error Buffer::UnreadByte()
{
    if (this->lastRead == opInvalid) {
        return errors::New(
                "bytes.Buffer: UnreadByte: previous operation was not a "
                "successful read");
    }
    this->lastRead = opInvalid;
    if (this->off > 0) {
        this->off--;
    }
    return nil;
}
std::tuple<moku::slice<uint8_t>, moku::error> Buffer::ReadBytes(uint8_t delim)
{
    moku::error err{};
    moku::slice<uint8_t> line{};
    moku::slice<uint8_t> slice{};

    std::tie(slice, err) = this->readSlice(delim);
    line = append(line, slice);
    return;
}
std::tuple<moku::slice<uint8_t>, moku::error> Buffer::readSlice(uint8_t delim)
{
    int end{0};
    moku::error err{};
    int i{0};
    moku::slice<uint8_t> line{};

    i = IndexByte(moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off),
                  delim);
    end = this->off + i + 1;
    if (i < 0) {
        end = len(this->buf);
        err = io::EOF;
    }
    line = moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->off, end);
    this->off = end;
    this->lastRead = opRead;
    return {line, err};
}
std::tuple<std::string, moku::error> Buffer::ReadString(uint8_t delim)
{
    moku::error err{};
    std::string line{""};
    moku::slice<uint8_t> slice{};

    std::tie(slice, err) = this->readSlice(delim);
    return {std::string(slice), err};
}
Buffer *NewBuffer(moku::slice<uint8_t> buf)
{
    return &Buffer{{buf, buf}};
}
Buffer *NewBufferString(std::string s)
{
    return &Buffer{{buf, moku::slice<byte>(s)}};
}
void init();
int main()
{
    init();
    ErrTooLarge = errors::New("bytes.Buffer: too large");
    _main();
    return 0;
}
