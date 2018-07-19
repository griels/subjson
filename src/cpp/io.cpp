std::tuple<int, moku::error> WriteString(Writer w, std::string s)
{
    moku::error err{};
    int n{0};

    {
        bool ok{false};
        stringWriter sw{};

        std::tie(sw, ok) = moku::try_assert<stringWriter>(w);

        if (ok) {
            return sw.WriteString(s);
        }
    }
    return w.Write(moku::slice<byte>(s));
}
std::tuple<int, moku::error> ReadAtLeast(Reader r,
                                         moku::slice<uint8_t> buf,
                                         int min)
{
    moku::error err{};
    int n{0};

    if (len(buf) < min) {
        return {0, ErrShortBuffer};
    }
    while (n < min && moku::is_nil(err)) {
        int nn{0};

        std::tie(nn, err) =
                r.Read(moku::slice_expr<moku::slice<uint8_t>>(buf, n));
        n += nn;
    }
    if (n >= min) {
        err = nil;
    } else {
        if (n > 0 && err == EOF) {
            err = ErrUnexpectedEOF;
        }
    }
    return;
}
std::tuple<int, moku::error> ReadFull(Reader r, moku::slice<uint8_t> buf)
{
    moku::error err{};
    int n{0};

    return ReadAtLeast(r, buf, len(buf));
}
std::tuple<int64_t, moku::error> CopyN(Writer dst, Reader src, int64_t n)
{
    moku::error err{};
    int64_t written{0};

    std::tie(written, err) = Copy(dst, LimitReader(src, n));
    if (written == n) {
        return {n, nil};
    }
    if (written < n && moku::is_nil(err)) {
        err = EOF;
    }
    return;
}
std::tuple<int64_t, moku::error> Copy(Writer dst, Reader src)
{
    moku::error err{};
    int64_t written{0};

    return copyBuffer(dst, src, nil);
}
std::tuple<int64_t, moku::error> CopyBuffer(Writer dst,
                                            Reader src,
                                            moku::slice<uint8_t> buf)
{
    moku::error err{};
    int64_t written{0};

    if (!moku::is_nil(buf) && len(buf) == 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 355002) STRING
                     "empty buffer in io.CopyBuffer"}));
    }
    return copyBuffer(dst, src, buf);
}
std::tuple<int64_t, moku::error> copyBuffer(Writer dst,
                                            Reader src,
                                            moku::slice<uint8_t> buf)
{
    moku::error err{};
    int64_t written{0};

    {
        bool ok{false};
        WriterTo wt{};

        std::tie(wt, ok) = moku::try_assert<WriterTo>(src);

        if (ok) {
            return wt.WriteTo(dst);
        }
    }
    {
        bool ok{false};
        ReaderFrom rt{};

        std::tie(rt, ok) = moku::try_assert<ReaderFrom>(dst);

        if (ok) {
            return rt.ReadFrom(src);
        }
    }
    if (moku::is_nil(buf)) {
        buf = make(moku::slice<byte>, 32 * 1024);
    }
    while (true) {
        moku::error er{};
        int nr{0};

        std::tie(nr, er) = src.Read(buf);
        if (nr > 0) {
            std::tie(nw, ew) = dst.Write(
                    moku::slice_expr<moku::slice<uint8_t>>(buf, 0, nr));
            if (nw > 0) {
                written += int64_t(nw);
            }
            if (!moku::is_nil(ew)) {
                err = ew;
                break;
            }
            if (nr != nw) {
                err = ErrShortWrite;
                break;
            }
        }
        if (!moku::is_nil(er)) {
            if (er != EOF) {
                err = er;
            }
            break;
        }
    }
    return {written, err};
}
Reader LimitReader(Reader r, int64_t n)
{
    return &LimitedReader{r, n};
}
std::tuple<int, moku::error> LimitedReader::Read(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    if (this->N <= 0) {
        return {0, EOF};
    }
    if (int64_t(len(p)) > this->N) {
        p = moku::slice_expr<moku::slice<uint8_t>>(p, 0, this->N);
    }
    std::tie(n, err) = this->R.Read(p);
    this->N -= int64_t(n);
    return;
}
SectionReader *NewSectionReader(ReaderAt r, int64_t off, int64_t n)
{
    return &SectionReader{r, off, off, off + n};
}
std::tuple<int, moku::error> SectionReader::Read(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    if (this->off >= this->limit) {
        return {0, EOF};
    }
    {
        int64_t max{0};

        max = this->limit - this->off;

        if (int64_t(len(p)) > max) {
            p = moku::slice_expr<moku::slice<uint8_t>>(p, 0, max);
        }
    }
    std::tie(n, err) = this->r.ReadAt(p, this->off);
    this->off += int64_t(n);
    return;
}
std::tuple<int64_t, moku::error> SectionReader::Seek(int64_t offset, int whence)
{
    if ((whence == SeekStart)) {
    _ident_1_:
        offset += this->base;
    } else if ((whence == SeekCurrent)) {
    _ident_2_:
        offset += this->off;
    } else if ((whence == SeekEnd)) {
    _ident_3_:
        offset += this->limit;
    } else {
    _ident_0_:
        return {0, errWhence};
    }
    if (offset < this->base) {
        return {0, errOffset};
    }
    this->off = offset;
    return {offset - this->base, nil};
}
std::tuple<int, moku::error> SectionReader::ReadAt(moku::slice<uint8_t> p,
                                                   int64_t off)
{
    moku::error err{};
    int n{0};

    if (off < 0 || off >= this->limit - this->base) {
        return {0, EOF};
    }
    off += this->base;
    {
        int64_t max{0};

        max = this->limit - off;

        if (int64_t(len(p)) > max) {
            p = moku::slice_expr<moku::slice<uint8_t>>(p, 0, max);
            std::tie(n, err) = this->r.ReadAt(p, off);
            if (moku::is_nil(err)) {
                err = EOF;
            }
            return {n, err};
        }
    }
    return this->r.ReadAt(p, off);
}
int64_t SectionReader::Size()
{
    return this->limit - this->base;
}
Reader TeeReader(Reader r, Writer w)
{
    return &teeReader{r, w};
}
std::tuple<int, moku::error> teeReader::Read(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    std::tie(n, err) = this->r.Read(p);
    if (n > 0) {
        {
            moku::error err{};
            int n{0};

            std::tie(n, err) =
                    this->w.Write(moku::slice_expr<moku::slice<uint8_t>>(p, n));

            if (!moku::is_nil(err)) {
                return {n, err};
            }
        }
    }
    return;
}
int main()
{
    ErrShortWrite = errors::New("short write");
    ErrShortBuffer = errors::New("short buffer");
    EOF = errors::New("EOF");
    ErrUnexpectedEOF = errors::New("unexpected EOF");
    ErrNoProgress = errors::New("multiple Read calls return no data or error");
    errWhence = errors::New("Seek: invalid whence");
    errOffset = errors::New("Seek: invalid offset");
    _ = (*multiWriter)(nil);
    ErrClosedPipe = errors::New("io: read/write on closed pipe");
    _main();
    return 0;
}
std::tuple<int, moku::error> eofReader::_ReadByValue(moku::slice<uint8_t>)
{
    return {0, EOF};
}
std::tuple<int, moku::error> multiReader::Read(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    while (len(this->readers) > 0) {
        if (len(this->readers) == 1) {
            {
                bool ok{false};
                multiReader *r{std::nullptr};

                std::tie(r, ok) =
                        moku::try_assert<*multiReader>(this->readers[0]);

                if (ok) {
                    this->readers = r->readers;
                    continue;
                }
            }
        }
        std::tie(n, err) = this->readers[0].Read(p);
        if (err == EOF) {
            this->readers[0] = eofReader{};
            this->readers =
                    moku::slice_expr<moku::slice<Reader>>(this->readers, 1);
        }
        if (n > 0 || err != EOF) {
            if (err == EOF && len(this->readers) > 0) {
                err = nil;
            }
            return;
        }
    }
    return {0, EOF};
}
Reader MultiReader(moku::slice<Reader> readers)
{
    moku::slice<Reader> r{};

    r = make(moku::slice<Reader>, len(readers));
    copy(r, readers);
    return &multiReader{r};
}
std::tuple<int, moku::error> multiWriter::Write(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    {
        Writer w{};
        for (w : moku::range_value<moku::slice<Writer>>(this->writers)) {
            std::tie(n, err) = w.Write(p);
            if (!moku::is_nil(err)) {
                return;
            }
            if (n != len(p)) {
                err = ErrShortWrite;
                return;
            }
        }
    }
    return {len(p), nil};
}
std::tuple<int, moku::error> multiWriter::WriteString(std::string s)
{
    moku::error err{};
    int n{0};
    moku::slice<uint8_t> p{};

    {
        Writer w{};
        for (w : moku::range_value<moku::slice<Writer>>(this->writers)) {
            {
                bool ok{false};
                stringWriter sw{};

                std::tie(sw, ok) = moku::try_assert<stringWriter>(w);

                if (ok) {
                    std::tie(n, err) = sw.WriteString(s);
                } else {
                    if (moku::is_nil(p)) {
                        p = moku::slice<byte>(s);
                    }
                    std::tie(n, err) = w.Write(p);
                }
            }
            if (!moku::is_nil(err)) {
                return;
            }
            if (n != len(s)) {
                err = ErrShortWrite;
                return;
            }
        }
    }
    return {len(s), nil};
}
Writer MultiWriter(moku::slice<Writer> writers)
{
    moku::slice<Writer> w{};

    w = make(moku::slice<Writer>, len(writers));
    copy(w, writers);
    return &multiWriter{w};
}
int main()
{
    ErrShortWrite = errors::New("short write");
    ErrShortBuffer = errors::New("short buffer");
    EOF = errors::New("EOF");
    ErrUnexpectedEOF = errors::New("unexpected EOF");
    ErrNoProgress = errors::New("multiple Read calls return no data or error");
    errWhence = errors::New("Seek: invalid whence");
    errOffset = errors::New("Seek: invalid offset");
    _ = (*multiWriter)(nil);
    ErrClosedPipe = errors::New("io: read/write on closed pipe");
    _main();
    return 0;
}
std::tuple<int, moku::error> pipe::read(moku::slice<uint8_t> b)
{
    moku::defer _defer_;
    moku::error err{};
    int n{0};

    this->rl.Lock();
    _defer_.Push([=]() mutable { this->rl.Unlock(); });
    this->l.Lock();
    _defer_.Push([=]() mutable { this->l.Unlock(); });
    while (true) {
        if (!moku::is_nil(this->rerr)) {
            return {0, ErrClosedPipe};
        }
        if (!moku::is_nil(this->data)) {
            break;
        }
        if (!moku::is_nil(this->werr)) {
            return {0, this->werr};
        }
        this->rwait.Wait();
    }
    n = copy(b, this->data);
    this->data = moku::slice_expr<moku::slice<uint8_t>>(this->data, n);
    if (len(this->data) == 0) {
        this->data = nil;
        this->wwait.Signal();
    }
    return;
}
std::tuple<int, moku::error> pipe::write(moku::slice<uint8_t> b)
{
    moku::defer _defer_;
    moku::error err{};
    int n{0};

    if (moku::is_nil(b)) {
        b = moku::slice_expr<std::vector<uint8_t>>(zero);
    }
    this->wl.Lock();
    _defer_.Push([=]() mutable { this->wl.Unlock(); });
    this->l.Lock();
    _defer_.Push([=]() mutable { this->l.Unlock(); });
    if (!moku::is_nil(this->werr)) {
        err = ErrClosedPipe;
        return;
    }
    this->data = b;
    this->rwait.Signal();
    while (true) {
        if (moku::is_nil(this->data)) {
            break;
        }
        if (!moku::is_nil(this->rerr)) {
            err = this->rerr;
            break;
        }
        if (!moku::is_nil(this->werr)) {
            err = ErrClosedPipe;
            break;
        }
        this->wwait.Wait();
    }
    n = len(b) - len(this->data);
    this->data = nil;
    return;
}
void pipe::rclose(moku::error err)
{
    moku::defer _defer_;

    if (moku::is_nil(err)) {
        err = ErrClosedPipe;
    }
    this->l.Lock();
    _defer_.Push([=]() mutable { this->l.Unlock(); });
    this->rerr = err;
    this->rwait.Signal();
    this->wwait.Signal();
}
void pipe::wclose(moku::error err)
{
    moku::defer _defer_;

    if (moku::is_nil(err)) {
        err = EOF;
    }
    this->l.Lock();
    _defer_.Push([=]() mutable { this->l.Unlock(); });
    this->werr = err;
    this->rwait.Signal();
    this->wwait.Signal();
}
std::tuple<int, moku::error> PipeReader::Read(moku::slice<uint8_t> data)
{
    moku::error err{};
    int n{0};

    return this->p->read(data);
}
moku::error PipeReader::Close()
{
    return this->CloseWithError(nil);
}
moku::error PipeReader::CloseWithError(moku::error err)
{
    this->p->rclose(err);
    return nil;
}
std::tuple<int, moku::error> PipeWriter::Write(moku::slice<uint8_t> data)
{
    moku::error err{};
    int n{0};

    return this->p->write(data);
}
moku::error PipeWriter::Close()
{
    return this->CloseWithError(nil);
}
moku::error PipeWriter::CloseWithError(moku::error err)
{
    this->p->wclose(err);
    return nil;
}
std::tuple<PipeReader *, PipeWriter *> Pipe()
{
    pipe *p{std::nullptr};
    PipeReader *r{std::nullptr};
    PipeWriter *w{std::nullptr};

    p = new (pipe);
    p->rwait.L = &p->l;
    p->wwait.L = &p->l;
    r = &PipeReader{p};
    w = &PipeWriter{p};
    return {r, w};
}
int main()
{
    ErrShortWrite = errors::New("short write");
    ErrShortBuffer = errors::New("short buffer");
    EOF = errors::New("EOF");
    ErrUnexpectedEOF = errors::New("unexpected EOF");
    ErrNoProgress = errors::New("multiple Read calls return no data or error");
    errWhence = errors::New("Seek: invalid whence");
    errOffset = errors::New("Seek: invalid offset");
    _ = (*multiWriter)(nil);
    ErrClosedPipe = errors::New("io: read/write on closed pipe");
    _main();
    return 0;
}
