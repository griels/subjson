Reader *NewReaderSize(Reader rd, int size)
{
    Reader *b{std::nullptr};
    bool ok{false};
    Reader *r{std::nullptr};

    std::tie(b, ok) = moku::try_assert<*Reader>(rd);
    if (ok && len(b->buf) >= size) {
        return b;
    }
    if (size < minReadBufferSize) {
        size = minReadBufferSize;
    }
    r = new (Reader);
    r->reset(make(moku::slice<byte>, size), rd);
    return r;
}
Reader *NewReader(Reader rd)
{
    return NewReaderSize(rd, defaultBufSize);
}
void Reader::Reset(Reader r)
{
    this->reset(this->buf, r);
}
void Reader::reset(moku::slice<uint8_t> buf, Reader r)
{
    *this = Reader{buf : buf, rd : r, lastByte : -1, lastRuneSize : -1};
}
void Reader::fill()
{
    if (this->r > 0) {
        copy(this->buf,
             moku::slice_expr<moku::slice<uint8_t>>(
                     this->buf, this->r, this->w));
        this->w -= this->r;
        this->r = 0;
    }
    if (this->w >= len(this->buf)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 4521495) STRING
                     "bufio: tried to fill full buffer"}));
    }
    {
        int i{0};
        for (i = maxConsecutiveEmptyReads; i > 0; i--) {
            moku::error err{};
            int n{0};

            std::tie(n, err) = this->rd.Read(
                    moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->w));
            if (n < 0) {
                panic(moku::make_iface<moku::error>(errNegativeRead));
            }
            this->w += n;
            if (!moku::is_nil(err)) {
                this->err = err;
                return;
            }
            if (n > 0) {
                return;
            }
        }
    }
    this->err = io::ErrNoProgress;
}
moku::error Reader::readErr()
{
    moku::error err{};

    err = this->err;
    this->err = nil;
    return err;
}
std::tuple<moku::slice<uint8_t>, moku::error> Reader::Peek(int n)
{
    moku::error err{};

    if (n < 0) {
        return {nil, ErrNegativeCount};
    }
    while (this->w - this->r < n && this->w - this->r < len(this->buf) &&
           moku::is_nil(this->err)) {
        this->fill();
    }
    if (n > len(this->buf)) {
        return {moku::slice_expr<moku::slice<uint8_t>>(
                        this->buf, this->r, this->w),
                ErrBufferFull};
    }
    {
        int avail{0};

        avail = this->w - this->r;

        if (avail < n) {
            n = avail;
            err = this->readErr();
            if (moku::is_nil(err)) {
                err = ErrBufferFull;
            }
        }
    }
    return {moku::slice_expr<moku::slice<uint8_t>>(
                    this->buf, this->r, this->r + n),
            err};
}
std::tuple<int, moku::error> Reader::Discard(int n)
{
    int discarded{0};
    moku::error err{};
    int remain{0};

    if (n < 0) {
        return {0, ErrNegativeCount};
    }
    if (n == 0) {
        return;
    }
    remain = n;
    while (true) {
        int skip{0};

        skip = this->Buffered();
        if (skip == 0) {
            this->fill();
            skip = this->Buffered();
        }
        if (skip > remain) {
            skip = remain;
        }
        this->r += skip;
        remain -= skip;
        if (remain == 0) {
            return {n, nil};
        }
        if (!moku::is_nil(this->err)) {
            return {n - remain, this->readErr()};
        }
    }
}
std::tuple<int, moku::error> Reader::Read(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    n = len(p);
    if (n == 0) {
        return {0, this->readErr()};
    }
    if (this->r == this->w) {
        if (!moku::is_nil(this->err)) {
            return {0, this->readErr()};
        }
        if (len(p) >= len(this->buf)) {
            std::tie(n, this->err) = this->rd.Read(p);
            if (n < 0) {
                panic(moku::make_iface<moku::error>(errNegativeRead));
            }
            if (n > 0) {
                this->lastByte = int(p[n - 1]);
                this->lastRuneSize = -1;
            }
            return {n, this->readErr()};
        }
        this->r = 0;
        this->w = 0;
        std::tie(n, this->err) = this->rd.Read(this->buf);
        if (n < 0) {
            panic(moku::make_iface<moku::error>(errNegativeRead));
        }
        if (n == 0) {
            return {0, this->readErr()};
        }
        this->w += n;
    }
    n = copy(p,
             moku::slice_expr<moku::slice<uint8_t>>(
                     this->buf, this->r, this->w));
    this->r += n;
    this->lastByte = int(this->buf[this->r - 1]);
    this->lastRuneSize = -1;
    return {n, nil};
}
std::tuple<uint8_t, moku::error> Reader::ReadByte()
{
    uint8_t c{0};

    this->lastRuneSize = -1;
    while (this->r == this->w) {
        if (!moku::is_nil(this->err)) {
            return {0, this->readErr()};
        }
        this->fill();
    }
    c = this->buf[this->r];
    this->r++;
    this->lastByte = int(c);
    return {c, nil};
}
moku::error Reader::UnreadByte()
{
    if (this->lastByte < 0 || this->r == 0 && this->w > 0) {
        return ErrInvalidUnreadByte;
    }
    if (this->r > 0) {
        this->r--;
    } else {
        this->w = 1;
    }
    this->buf[this->r] = byte(this->lastByte);
    this->lastByte = -1;
    this->lastRuneSize = -1;
    return nil;
}
std::tuple<int32_t, int, moku::error> Reader::ReadRune()
{
    moku::error err{};
    int32_t r{0};
    int size{0};

    while (this->r + utf8::UTFMax > this->w &&
           !utf8::FullRune(moku::slice_expr<moku::slice<uint8_t>>(
                   this->buf, this->r, this->w)) &&
           moku::is_nil(this->err) && this->w - this->r < len(this->buf)) {
        this->fill();
    }
    this->lastRuneSize = -1;
    if (this->r == this->w) {
        return {0, 0, this->readErr()};
    }
    std::tie(r, size) = std::tuple<int32_t, int>(rune(this->buf[this->r]), 1);
    if (r >= utf8::RuneSelf) {
        std::tie(r, size) =
                utf8::DecodeRune(moku::slice_expr<moku::slice<uint8_t>>(
                        this->buf, this->r, this->w));
    }
    this->r += size;
    this->lastByte = int(this->buf[this->r - 1]);
    this->lastRuneSize = size;
    return {r, size, nil};
}
moku::error Reader::UnreadRune()
{
    if (this->lastRuneSize < 0 || this->r < this->lastRuneSize) {
        return ErrInvalidUnreadRune;
    }
    this->r -= this->lastRuneSize;
    this->lastByte = -1;
    this->lastRuneSize = -1;
    return nil;
}
int Reader::Buffered()
{
    return this->w - this->r;
}
std::tuple<moku::slice<uint8_t>, moku::error> Reader::ReadSlice(uint8_t delim)
{
    moku::error err{};
    moku::slice<uint8_t> line{};

    while (true) {
        {
            int i{0};

            i = bytes::IndexByte(moku::slice_expr<moku::slice<uint8_t>>(
                                         this->buf, this->r, this->w),
                                 delim);

            if (i >= 0) {
                line = moku::slice_expr<moku::slice<uint8_t>>(
                        this->buf, this->r, this->r + i + 1);
                this->r += i + 1;
                break;
            }
        }
        if (!moku::is_nil(this->err)) {
            line = moku::slice_expr<moku::slice<uint8_t>>(
                    this->buf, this->r, this->w);
            this->r = this->w;
            err = this->readErr();
            break;
        }
        if (this->Buffered() >= len(this->buf)) {
            this->r = this->w;
            line = this->buf;
            err = ErrBufferFull;
            break;
        }
        this->fill();
    }
    {
        int i{0};

        i = len(line) - 1;

        if (i >= 0) {
            this->lastByte = int(line[i]);
            this->lastRuneSize = -1;
        }
    }
    return;
}
std::tuple<moku::slice<uint8_t>, bool, moku::error> Reader::ReadLine()
{
    moku::error err{};
    bool isPrefix{false};
    moku::slice<uint8_t> line{};

    std::tie(line, err) = this->ReadSlice('\n');
    if (err == ErrBufferFull) {
        if (len(line) > 0 && line[len(line) - 1] == '\r') {
            if (this->r == 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 4528976) STRING
                             "bufio: tried to rewind past start of buffer"}));
            }
            this->r--;
            line = moku::slice_expr<moku::slice<uint8_t>>(line, len(line) - 1);
        }
        return {line, true, nil};
    }
    if (len(line) == 0) {
        if (!moku::is_nil(err)) {
            line = nil;
        }
        return;
    }
    err = nil;
    if (line[len(line) - 1] == '\n') {
        drop = 1;
        if (len(line) > 1 && line[len(line) - 2] == '\r') {
            drop = 2;
        }
        line = moku::slice_expr<moku::slice<uint8_t>>(line, len(line) - drop);
    }
    return;
}
std::tuple<moku::slice<uint8_t>, moku::error> Reader::ReadBytes(uint8_t delim)
{
    moku::slice<uint8_t> buf{};
    moku::error err{};
    moku::slice<uint8_t> frag{};
    moku::slice<moku::slice<uint8_t>> full{};
    int n{0};

    while (true) {
        moku::slice<uint8_t> buf{};
        moku::error e{};

        std::tie(frag, e) = this->ReadSlice(delim);
        if (moku::is_nil(e)) {
            break;
        }
        if (e != ErrBufferFull) {
            err = e;
            break;
        }
        buf = make(moku::slice<byte>, len(frag));
        copy(buf, frag);
        full = append(full, buf);
    }
    n = 0;
    {
        int i{0};
        for (i : moku::range_key<moku::slice<moku::slice<uint8_t>>>(full)) {
            n += len(full[i]);
        }
    }
    n += len(frag);
    buf = make(moku::slice<byte>, n);
    n = 0;
    {
        int i{0};
        for (i : moku::range_key<moku::slice<moku::slice<uint8_t>>>(full)) {
            n += copy(moku::slice_expr<moku::slice<uint8_t>>(buf, n), full[i]);
        }
    }
    copy(moku::slice_expr<moku::slice<uint8_t>>(buf, n), frag);
    return {buf, err};
}
std::tuple<std::string, moku::error> Reader::ReadString(uint8_t delim)
{
    moku::slice<uint8_t> bytes{};
    moku::error err{};

    std::tie(bytes, err) = this->ReadBytes(delim);
    return {std::string(bytes), err};
}
std::tuple<int64_t, moku::error> Reader::WriteTo(Writer w)
{
    moku::error err{};
    int64_t n{0};

    std::tie(n, err) = this->writeBuf(w);
    if (!moku::is_nil(err)) {
        return;
    }
    {
        bool ok{false};
        WriterTo r{};

        std::tie(r, ok) = moku::try_assert<io::WriterTo>(this->rd);

        if (ok) {
            std::tie(m, err) = r.WriteTo(w);
            n += m;
            return {n, err};
        }
    }
    {
        bool ok{false};
        ReaderFrom w{};

        std::tie(w, ok) = moku::try_assert<io::ReaderFrom>(w);

        if (ok) {
            std::tie(m, err) = w.ReadFrom(this->rd);
            n += m;
            return {n, err};
        }
    }
    if (this->w - this->r < len(this->buf)) {
        this->fill();
    }
    while (this->r < this->w) {
        moku::error err{};
        int64_t m{0};

        std::tie(m, err) = this->writeBuf(w);
        n += m;
        if (!moku::is_nil(err)) {
            return {n, err};
        }
        this->fill();
    }
    if (this->err == io::EOF) {
        this->err = nil;
    }
    return {n, this->readErr()};
}
std::tuple<int64_t, moku::error> Reader::writeBuf(Writer w)
{
    moku::error err{};
    int n{0};

    std::tie(n, err) = w.Write(moku::slice_expr<moku::slice<uint8_t>>(
            this->buf, this->r, this->w));
    if (n < 0) {
        panic(moku::make_iface<moku::error>(errNegativeWrite));
    }
    this->r += n;
    return {int64_t(n), err};
}
Writer *NewWriterSize(Writer w, int size)
{
    Writer *b{std::nullptr};
    bool ok{false};

    std::tie(b, ok) = moku::try_assert<*Writer>(w);
    if (ok && len(b->buf) >= size) {
        return b;
    }
    if (size <= 0) {
        size = defaultBufSize;
    }
    return &Writer{{buf, make(moku::slice<byte>, size)}, {wr, w}};
}
Writer *NewWriter(Writer w)
{
    return NewWriterSize(w, defaultBufSize);
}
void Writer::Reset(Writer w)
{
    this->err = nil;
    this->n = 0;
    this->wr = w;
}
moku::error Writer::Flush()
{
    moku::error err{};
    int n{0};

    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    if (this->n == 0) {
        return nil;
    }
    std::tie(n, err) = this->wr.Write(
            moku::slice_expr<moku::slice<uint8_t>>(this->buf, 0, this->n));
    if (n < this->n && moku::is_nil(err)) {
        err = io::ErrShortWrite;
    }
    if (!moku::is_nil(err)) {
        if (n > 0 && n < this->n) {
            copy(moku::slice_expr<moku::slice<uint8_t>>(
                         this->buf, 0, this->n - n),
                 moku::slice_expr<moku::slice<uint8_t>>(this->buf, n, this->n));
        }
        this->n -= n;
        this->err = err;
        return err;
    }
    this->n = 0;
    return nil;
}
int Writer::Available()
{
    return len(this->buf) - this->n;
}
int Writer::Buffered()
{
    return this->n;
}
std::tuple<int, moku::error> Writer::Write(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};
    int nn{0};

    while (len(p) > this->Available() && moku::is_nil(this->err)) {
        int n{0};

        if (this->Buffered() == 0) {
            std::tie(n, this->err) = this->wr.Write(p);
        } else {
            n = copy(moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->n),
                     p);
            this->n += n;
            this->Flush();
        }
        nn += n;
        p = moku::slice_expr<moku::slice<uint8_t>>(p, n);
    }
    if (!moku::is_nil(this->err)) {
        return {nn, this->err};
    }
    n = copy(moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->n), p);
    this->n += n;
    nn += n;
    return {nn, nil};
}
moku::error Writer::WriteByte(uint8_t c)
{
    if (!moku::is_nil(this->err)) {
        return this->err;
    }
    if (this->Available() <= 0 && !moku::is_nil(this->Flush())) {
        return this->err;
    }
    this->buf[this->n] = c;
    this->n++;
    return nil;
}
std::tuple<int, moku::error> Writer::WriteRune(int32_t r)
{
    moku::error err{};
    int n{0};
    int size{0};

    if (r < utf8::RuneSelf) {
        err = this->WriteByte(byte(r));
        if (!moku::is_nil(err)) {
            return {0, err};
        }
        return {1, nil};
    }
    if (!moku::is_nil(this->err)) {
        return {0, this->err};
    }
    n = this->Available();
    if (n < utf8::UTFMax) {
        {
            this->Flush();

            if (!moku::is_nil(this->err)) {
                return {0, this->err};
            }
        }
        n = this->Available();
        if (n < utf8::UTFMax) {
            return this->WriteString(std::string(r));
        }
    }
    size = utf8::EncodeRune(
            moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->n), r);
    this->n += size;
    return {size, nil};
}
std::tuple<int, moku::error> Writer::WriteString(std::string s)
{
    int n{0};
    int nn{0};

    nn = 0;
    while (len(s) > this->Available() && moku::is_nil(this->err)) {
        int n{0};

        n = copy(moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->n), s);
        this->n += n;
        nn += n;
        s = moku::slice_expr<std::string>(s, n);
        this->Flush();
    }
    if (!moku::is_nil(this->err)) {
        return {nn, this->err};
    }
    n = copy(moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->n), s);
    this->n += n;
    nn += n;
    return {nn, nil};
}
std::tuple<int64_t, moku::error> Writer::ReadFrom(Reader r)
{
    moku::error err{};
    int m{0};
    int64_t n{0};

    if (this->Buffered() == 0) {
        {
            bool ok{false};
            ReaderFrom w{};

            std::tie(w, ok) = moku::try_assert<io::ReaderFrom>(this->wr);

            if (ok) {
                return w.ReadFrom(r);
            }
        }
    }
    while (true) {
        int nr{0};

        if (this->Available() == 0) {
            {
                moku::error err1{};

                err1 = this->Flush();

                if (!moku::is_nil(err1)) {
                    return {n, err1};
                }
            }
        }
        nr = 0;
        while (nr < maxConsecutiveEmptyReads) {
            std::tie(m, err) = r.Read(
                    moku::slice_expr<moku::slice<uint8_t>>(this->buf, this->n));
            if (m != 0 || !moku::is_nil(err)) {
                break;
            }
            nr++;
        }
        if (nr == maxConsecutiveEmptyReads) {
            return {n, io::ErrNoProgress};
        }
        this->n += m;
        n += int64_t(m);
        if (!moku::is_nil(err)) {
            break;
        }
    }
    if (err == io::EOF) {
        if (this->Available() == 0) {
            err = this->Flush();
        } else {
            err = nil;
        }
    }
    return {n, err};
}
ReadWriter *NewReadWriter(Reader *r, Writer *w)
{
    return &ReadWriter{r, w};
}
int main()
{
    ErrInvalidUnreadByte = errors::New("bufio: invalid use of UnreadByte");
    ErrInvalidUnreadRune = errors::New("bufio: invalid use of UnreadRune");
    ErrBufferFull = errors::New("bufio: buffer full");
    ErrNegativeCount = errors::New("bufio: negative count");
    errNegativeRead =
            errors::New("bufio: reader returned negative count from Read");
    errNegativeWrite =
            errors::New("bufio: writer returned negative count from Write");
    ErrTooLong = errors::New("bufio.Scanner: token too long");
    ErrNegativeAdvance = errors::New(
            "bufio.Scanner: SplitFunc returns negative advance count");
    ErrAdvanceTooFar = errors::New(
            "bufio.Scanner: SplitFunc returns advance count beyond input");
    ErrFinalToken = errors::New("final token");
    errorRune = moku::slice<byte>(std::string(utf8::RuneError));
    _main();
    return 0;
}
