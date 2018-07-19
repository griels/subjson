void NopResetter::_ResetByValue()
{
}
Reader *NewReader(Reader r, Transformer t)
{
    t.Reset();
    return &Reader{{r, r},
                   {t, t},
                   {dst, make(moku::slice<byte>, defaultBufSize)},
                   {src, make(moku::slice<byte>, defaultBufSize)}};
}
std::tuple<int, moku::error> Reader::Read(moku::slice<uint8_t> p)
{
    moku::error err{};
    int n{0};

    std::tie(n, err) = std::tuple<int, moku::error>(0, error(nil));
    while (true) {
        if (this->dst0 != this->dst1) {
            n = copy(p,
                     moku::slice_expr<moku::slice<uint8_t>>(
                             this->dst, this->dst0, this->dst1));
            this->dst0 += n;
            if (this->dst0 == this->dst1 && this->transformComplete) {
                return {n, this->err};
            }
            return {n, nil};
        } else {
            if (this->transformComplete) {
                return {0, this->err};
            }
        }
        if (this->src0 != this->src1 || !moku::is_nil(this->err)) {
            this->dst0 = 0;
            std::tie(this->dst1, n, err) = this->t.Transform(
                    this->dst,
                    moku::slice_expr<moku::slice<uint8_t>>(
                            this->src, this->src0, this->src1),
                    this->err == io::EOF);
            this->src0 += n;
            if ((moku::is_nil(err))) {
            _ident_0_:
                if (this->src0 != this->src1) {
                    this->err = errInconsistentByteCount;
                }
                this->transformComplete = !moku::is_nil(this->err);
                continue;
            } else if ((err == ErrShortDst && (this->dst1 != 0 || n != 0))) {
            _ident_1_:
                continue;
            } else if ((err == ErrShortSrc &&
                        this->src1 - this->src0 != len(this->src) &&
                        moku::is_nil(this->err))) {
            _ident_2_:
            } else {
            _ident_3_:
                this->transformComplete = true;
                if (moku::is_nil(this->err) || this->err == io::EOF) {
                    this->err = err;
                }
                continue;
            }
        }
        if (this->src0 != 0) {
            std::tie(this->src0, this->src1) = std::tuple<int, int>(
                    0,
                    copy(this->src,
                         moku::slice_expr<moku::slice<uint8_t>>(
                                 this->src, this->src0, this->src1)));
        }
        std::tie(n, this->err) = this->r.Read(
                moku::slice_expr<moku::slice<uint8_t>>(this->src, this->src1));
        this->src1 += n;
    }
}
Writer *NewWriter(Writer w, Transformer t)
{
    t.Reset();
    return &Writer{{w, w},
                   {t, t},
                   {dst, make(moku::slice<byte>, defaultBufSize)},
                   {src, make(moku::slice<byte>, defaultBufSize)}};
}
std::tuple<int, moku::error> Writer::Write(moku::slice<uint8_t> data)
{
    moku::error err{};
    int n{0};
    moku::slice<uint8_t> src{};

    src = data;
    if (this->n > 0) {
        n = copy(moku::slice_expr<moku::slice<uint8_t>>(this->src, this->n),
                 data);
        this->n += n;
        src = moku::slice_expr<moku::slice<uint8_t>>(this->src, this->n);
    }
    while (true) {
        moku::error err{};
        int nDst{0};
        int nSrc{0};

        std::tie(nDst, nSrc, err) = this->t.Transform(this->dst, src, false);
        {
            moku::error werr{};

            std::tie(_, werr) = this->w.Write(
                    moku::slice_expr<moku::slice<uint8_t>>(this->dst, nDst));

            if (!moku::is_nil(werr)) {
                return {n, werr};
            }
        }
        src = moku::slice_expr<moku::slice<uint8_t>>(src, nSrc);
        if (this->n == 0) {
            n += nSrc;
        } else {
            if (len(src) <= n) {
                this->n = 0;
                n -= len(src);
                src = moku::slice_expr<moku::slice<uint8_t>>(data, n);
                if (n < len(data) &&
                    (moku::is_nil(err) || err == ErrShortSrc)) {
                    continue;
                }
            }
        }
        if ((err == ErrShortDst)) {
        _ident_4_:
            if (nDst > 0 || nSrc > 0) {
                continue;
            }
        } else if ((err == ErrShortSrc)) {
        _ident_5_:
            if (len(src) < len(this->src)) {
                m = copy(this->src, src);
                if (this->n == 0) {
                    n += m;
                }
                this->n = m;
                err = nil;
            } else {
                if (nDst > 0 || nSrc > 0) {
                    continue;
                }
            }
        } else if ((err == nil)) {
        _ident_6_:
            if (this->n > 0) {
                err = errInconsistentByteCount;
            }
        }
        return {n, err};
    }
}
moku::error Writer::Close()
{
    moku::slice<uint8_t> src{};

    src = moku::slice_expr<moku::slice<uint8_t>>(this->src, this->n);
    while (true) {
        moku::error err{};
        int nDst{0};
        int nSrc{0};

        std::tie(nDst, nSrc, err) = this->t.Transform(this->dst, src, true);
        {
            moku::error werr{};

            std::tie(_, werr) = this->w.Write(
                    moku::slice_expr<moku::slice<uint8_t>>(this->dst, nDst));

            if (!moku::is_nil(werr)) {
                return werr;
            }
        }
        if (err != ErrShortDst) {
            return err;
        }
        src = moku::slice_expr<moku::slice<uint8_t>>(src, nSrc);
    }
}
std::tuple<int, int, moku::error> nop::_TransformByValue(
        moku::slice<uint8_t> dst, moku::slice<uint8_t> src, bool atEOF)
{
    moku::error err{};
    int n{0};
    int nDst{0};
    int nSrc{0};

    n = copy(dst, src);
    if (n < len(src)) {
        err = ErrShortDst;
    }
    return {n, n, err};
}
std::tuple<int, moku::error> nop::_SpanByValue(moku::slice<uint8_t> src,
                                               bool atEOF)
{
    moku::error err{};
    int n{0};

    return {len(src), nil};
}
std::tuple<int, int, moku::error> discard::_TransformByValue(
        moku::slice<uint8_t> dst, moku::slice<uint8_t> src, bool atEOF)
{
    moku::error err{};
    int nDst{0};
    int nSrc{0};

    return {0, len(src), nil};
}
void chain::fatalError(int errIndex, moku::error err)
{
    {
        int i{0};

        i = errIndex + 1;

        if (i > this->errStart) {
            this->errStart = i;
            this->err = err;
        }
    }
}
moku::slice<uint8_t> link::src()
{
    return moku::slice_expr<moku::slice<uint8_t>>(this->b, this->p, this->n);
}
moku::slice<uint8_t> link::dst()
{
    return moku::slice_expr<moku::slice<uint8_t>>(this->b, this->n);
}
Transformer Chain(moku::slice<Transformer> t)
{
    moku::slice<std::vector<uint8_t>> b{};
    chain *c{std::nullptr};

    if (len(t) == 0) {
        return nop{};
    }
    c = &chain{{link, make(moku::slice<link>, len(t) + 1)}};
    {
        int i{0};
        Transformer tt{};
        for (std::tie(i, tt) :
             moku::range_key_value<moku::slice<Transformer>>(t)) {
            c->link[i].t = tt;
        }
    }
    b = make(moku::slice<std::vector<byte>>, len(t) - 1);
    {
        int i{0};
        for (i : moku::range_key<moku::slice<std::vector<uint8_t>>>(b)) {
            c->link[i + 1].b = moku::slice_expr<std::vector<uint8_t>>(b[i]);
        }
    }
    return c;
}
void chain::Reset()
{
    {
        int i{0};
        link l{};
        for (std::tie(i, l) :
             moku::range_key_value<moku::slice<link>>(this->link)) {
            if (!moku::is_nil(l.t)) {
                l.t.Reset();
            }
            std::tie(this->link[i].p, this->link[i].n) =
                    std::tuple<int, int>(0, 0);
        }
    }
}
std::tuple<int, int, moku::error> chain::Transform(moku::slice<uint8_t> dst,
                                                   moku::slice<uint8_t> src,
                                                   bool atEOF)
{
}
