PublicKey PrivateKey::Public()
{
    return &this->PublicKey;
}
std::tuple<moku::slice<uint8_t>, moku::error> PrivateKey::Sign(
        Reader rand, moku::slice<uint8_t> msg, SignerOpts opts)
{
    moku::error err{};
    Int *r{std::nullptr};
    Int *s{std::nullptr};

    std::tie(r, s, err) = Sign(rand, this, msg);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    return asn1::Marshal(moku::make_iface<ecdsaSignature>(
            &{ecdsaSignature % !s(token.Pos = 6273402)[r s] %
              !s(token.Pos = 6273407)}));
}
std::tuple<Int *, moku::error> randFieldElement(Curve c, Reader rand)
{
    moku::slice<uint8_t> b{};
    moku::error err{};
    Int *k{std::nullptr};
    Int *n{std::nullptr};
    CurveParams *params{std::nullptr};

    params = c.Params();
    b = make(moku::slice<byte>, params->BitSize / 8 + 8);
    std::tie(_, err) = io::ReadFull(rand, b);
    if (!moku::is_nil(err)) {
        return;
    }
    k = new (big::Int)->SetBytes(b);
    n = new (big::Int)->Sub(params->N, one);
    k->Mod(k, n);
    k->Add(k, one);
    return;
}
std::tuple<PrivateKey *, moku::error> GenerateKey(Curve c, Reader rand)
{
    moku::error err{};
    Int *k{std::nullptr};
    PrivateKey *priv{std::nullptr};

    std::tie(k, err) = randFieldElement(c, rand);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    priv = new (PrivateKey);
    priv->PublicKey.Curve = c;
    priv->D = k;
    std::tie(priv->PublicKey.X, priv->PublicKey.Y) =
            c.ScalarBaseMult(k->Bytes());
    return {priv, nil};
}
Int *hashToInt(moku::slice<uint8_t> hash, Curve c)
{
    int excess{0};
    int orderBits{0};
    int orderBytes{0};
    Int *ret{std::nullptr};

    orderBits = c.Params()->N->BitLen();
    orderBytes = (orderBits + 7) / 8;
    if (len(hash) > orderBytes) {
        hash = moku::slice_expr<moku::slice<uint8_t>>(hash, orderBytes);
    }
    ret = new (big::Int)->SetBytes(hash);
    excess = len(hash) * 8 - orderBits;
    if (excess > 0) {
        ret->Rsh(ret, unsigned int(excess));
    }
    return ret;
}
Int *fermatInverse(Int *k, Int *N)
{
    Int *nMinus2{std::nullptr};
    Int *two{std::nullptr};

    two = big::NewInt(2);
    nMinus2 = new (big::Int)->Sub(N, two);
    return new (big::Int)->Exp(k, nMinus2, N);
}
std::tuple<Int *, Int *, moku::error> Sign(Reader rand,
                                           PrivateKey *priv,
                                           moku::slice<uint8_t> hash)
{
    Int *N{std::nullptr};
    Block block{};
    Curve c{};
    StreamReader csprng{};
    moku::slice<uint8_t> entropy{};
    int entropylen{0};
    moku::error err{};
    Int *k{std::nullptr};
    Int *kInv{std::nullptr};
    moku::slice<uint8_t> key{};
    Hash md{};
    Int *r{std::nullptr};
    Int *s{std::nullptr};

    entropylen = (priv->Curve.Params()->BitSize + 7) / 16;
    if (entropylen > 32) {
        entropylen = 32;
    }
    entropy = make(moku::slice<byte>, entropylen);
    std::tie(_, err) = io::ReadFull(rand, entropy);
    if (!moku::is_nil(err)) {
        return;
    }
    md = sha512::New();
    md.Write(priv->D->Bytes());
    md.Write(entropy);
    md.Write(hash);
    key = moku::slice_expr<moku::slice<uint8_t>>(md.Sum(nil), 32);
    std::tie(block, err) = aes::NewCipher(key);
    if (!moku::is_nil(err)) {
        return {nil, nil, err};
    }
    csprng = cipher::StreamReader{
        R : zeroReader,
        S : cipher::NewCTR(block, moku::slice<byte>(aesIV))
    };
    c = priv->PublicKey.Curve;
    N = c.Params()->N;
    if (N->Sign() == 0) {
        return {nil, nil, errZeroParam};
    }
    while (true) {
        Int *e{std::nullptr};

        while (true) {
            std::tie(k, err) = randFieldElement(c, csprng);
            if (!moku::is_nil(err)) {
                r = nil;
                return;
            }
            {
                invertible in{};
                bool ok{false};

                std::tie(in, ok) = moku::try_assert<invertible>(priv->Curve);

                if (ok) {
                    kInv = in.Inverse(k);
                } else {
                    kInv = fermatInverse(k, N);
                }
            }
            std::tie(r, _) = priv->Curve.ScalarBaseMult(k->Bytes());
            r->Mod(r, N);
            if (r->Sign() != 0) {
                break;
            }
        }
        e = hashToInt(hash, c);
        s = new (big::Int)->Mul(priv->D, r);
        s->Add(s, e);
        s->Mul(s, kInv);
        s->Mod(s, N);
        if (s->Sign() != 0) {
            break;
        }
    }
    return;
}
bool Verify(PublicKey *pub, moku::slice<uint8_t> hash, Int *r, Int *s)
{
    Int *N{std::nullptr};
    Curve c{};
    Int *e{std::nullptr};
    Int *u1{std::nullptr};
    Int *u2{std::nullptr};
    Int *w{std::nullptr};
    Int *x{std::nullptr};
    Int *y{std::nullptr};

    c = pub->Curve;
    N = c.Params()->N;
    if (r->Sign() <= 0 || s->Sign() <= 0) {
        return false;
    }
    if (r->Cmp(N) >= 0 || s->Cmp(N) >= 0) {
        return false;
    }
    e = hashToInt(hash, c);
    {
        invertible in{};
        bool ok{false};

        std::tie(in, ok) = moku::try_assert<invertible>(c);

        if (ok) {
            w = in.Inverse(s);
        } else {
            w = new (big::Int)->ModInverse(s, N);
        }
    }
    u1 = e->Mul(e, w);
    u1->Mod(u1, N);
    u2 = w->Mul(r, w);
    u2->Mod(u2, N);
    {
        bool ok{false};
        combinedMult opt{};

        std::tie(opt, ok) = moku::try_assert<combinedMult>(c);

        if (ok) {
            std::tie(x, y) =
                    opt.CombinedMult(pub->X, pub->Y, u1->Bytes(), u2->Bytes());
        } else {
            std::tie(x1, y1) = c.ScalarBaseMult(u1->Bytes());
            std::tie(x2, y2) = c.ScalarMult(pub->X, pub->Y, u2->Bytes());
            std::tie(x, y) = c.Add(x1, y1, x2, y2);
        }
    }
    if (x->Sign() == 0 && y->Sign() == 0) {
        return false;
    }
    x->Mod(x, N);
    return x->Cmp(r) == 0;
}
std::tuple<int, moku::error> zr::Read(moku::slice<uint8_t> dst)
{
    moku::error err{};
    int n{0};

    {
        int i{0};
        for (i : moku::range_key<moku::slice<uint8_t>>(dst)) {
            dst[i] = 0;
        }
    }
    return {len(dst), nil};
}
int main()
{
    one = new (big::Int)->SetInt64(1);
    errZeroParam = errors::New("zero parameter");
    zeroReader = &zr{};
    _main();
    return 0;
}
