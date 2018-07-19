void init()
{
    crypto::RegisterHash(crypto::SHA384, New384);
    crypto::RegisterHash(crypto::SHA512, New);
    crypto::RegisterHash(crypto::SHA512_224, New512_224);
    crypto::RegisterHash(crypto::SHA512_256, New512_256);
}
void digest::Reset()
{
    if ((this->function == crypto::SHA384)) {
    _ident_0_:
        this->h[0] = init0_384;
        this->h[1] = init1_384;
        this->h[2] = init2_384;
        this->h[3] = init3_384;
        this->h[4] = init4_384;
        this->h[5] = init5_384;
        this->h[6] = init6_384;
        this->h[7] = init7_384;
    } else if ((this->function == crypto::SHA512_224)) {
    _ident_1_:
        this->h[0] = init0_224;
        this->h[1] = init1_224;
        this->h[2] = init2_224;
        this->h[3] = init3_224;
        this->h[4] = init4_224;
        this->h[5] = init5_224;
        this->h[6] = init6_224;
        this->h[7] = init7_224;
    } else if ((this->function == crypto::SHA512_256)) {
    _ident_2_:
        this->h[0] = init0_256;
        this->h[1] = init1_256;
        this->h[2] = init2_256;
        this->h[3] = init3_256;
        this->h[4] = init4_256;
        this->h[5] = init5_256;
        this->h[6] = init6_256;
        this->h[7] = init7_256;
    } else {
    _ident_3_:
        this->h[0] = init0;
        this->h[1] = init1;
        this->h[2] = init2;
        this->h[3] = init3;
        this->h[4] = init4;
        this->h[5] = init5;
        this->h[6] = init6;
        this->h[7] = init7;
    }
    this->nx = 0;
    this->len = 0;
}
Hash New()
{
    digest *d{std::nullptr};

    d = &digest{{function, crypto::SHA512}};
    d->Reset();
    return d;
}
Hash New512_224()
{
    digest *d{std::nullptr};

    d = &digest{{function, crypto::SHA512_224}};
    d->Reset();
    return d;
}
Hash New512_256()
{
    digest *d{std::nullptr};

    d = &digest{{function, crypto::SHA512_256}};
    d->Reset();
    return d;
}
Hash New384()
{
    digest *d{std::nullptr};

    d = &digest{{function, crypto::SHA384}};
    d->Reset();
    return d;
}
int digest::Size()
{
    if ((this->function == crypto::SHA512_224)) {
    _ident_4_:
        return Size224;
    } else if ((this->function == crypto::SHA512_256)) {
    _ident_5_:
        return Size256;
    } else if ((this->function == crypto::SHA384)) {
    _ident_6_:
        return Size384;
    } else {
    _ident_7_:
        return Size;
    }
}
int digest::BlockSize()
{
    return BlockSize;
}
std::tuple<int, moku::error> digest::Write(moku::slice<uint8_t> p)
{
    moku::error err{};
    int nn{0};

    nn = len(p);
    this->len += uint64_t(nn);
    if (this->nx > 0) {
        n = copy(moku::slice_expr<std::vector<uint8_t>>(this->x, this->nx), p);
        this->nx += n;
        if (this->nx == chunk) {
            block(this, moku::slice_expr<std::vector<uint8_t>>(this->x));
            this->nx = 0;
        }
        p = moku::slice_expr<moku::slice<uint8_t>>(p, n);
    }
    if (len(p) >= chunk) {
        n = len(p) & ~((chunk - 1));
        block(this, moku::slice_expr<moku::slice<uint8_t>>(p, n));
        p = moku::slice_expr<moku::slice<uint8_t>>(p, n);
    }
    if (len(p) > 0) {
        this->nx = copy(moku::slice_expr<std::vector<uint8_t>>(this->x), p);
    }
    return;
}
moku::slice<uint8_t> digest::Sum(moku::slice<uint8_t> in)
{
    digest *d{std::nullptr};
    std::vector<uint8_t> hash{};

    d = new (digest);
    *d = *this;
    hash = d->checkSum();
    if ((d->function == crypto::SHA384)) {
    _ident_8_:
        return append(in,
                      moku::slice_expr<std::vector<uint8_t>>(hash, Size384));
    } else if ((d->function == crypto::SHA512_224)) {
    _ident_9_:
        return append(in,
                      moku::slice_expr<std::vector<uint8_t>>(hash, Size224));
    } else if ((d->function == crypto::SHA512_256)) {
    _ident_10_:
        return append(in,
                      moku::slice_expr<std::vector<uint8_t>>(hash, Size256));
    } else {
    _ident_11_:
        return append(in, moku::slice_expr<std::vector<uint8_t>>(hash));
    }
}
std::vector<uint8_t> digest::checkSum()
{
    std::vector<uint8_t> digest{};
    moku::slice<uint64_t> h{};
    uint64_t len{0};
    std::vector<uint8_t> tmp{};

    len = this->len;
    tmp[0] = 0x80;
    if (len % 128 < 112) {
        this->Write(moku::slice_expr<std::vector<uint8_t>>(
                tmp, 0, 112 - len % 128));
    } else {
        this->Write(moku::slice_expr<std::vector<uint8_t>>(
                tmp, 0, 128 + 112 - len % 128));
    }
    len <<= 3;
    {
        unsigned int i{0};
        for (i = unsigned int(0); i < 16; i++) {
            tmp[i] = byte(len >> (120 - 8 * i));
        }
    }
    this->Write(moku::slice_expr<std::vector<uint8_t>>(tmp, 0, 16));
    if (this->nx != 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 6284916) STRING "d.nx != 0"}));
    }
    h = moku::slice_expr<std::vector<uint64_t>>(this->h);
    if (this->function == crypto::SHA384) {
        h = moku::slice_expr<std::vector<uint64_t>>(this->h, 6);
    }
    {
        int i{0};
        uint64_t s{0};
        for (std::tie(i, s) : moku::range_key_value<moku::slice<uint64_t>>(h)) {
            digest[i * 8] = byte(s >> 56);
            digest[i * 8 + 1] = byte(s >> 48);
            digest[i * 8 + 2] = byte(s >> 40);
            digest[i * 8 + 3] = byte(s >> 32);
            digest[i * 8 + 4] = byte(s >> 24);
            digest[i * 8 + 5] = byte(s >> 16);
            digest[i * 8 + 6] = byte(s >> 8);
            digest[i * 8 + 7] = byte(s);
        }
    }
    return digest;
}
std::vector<uint8_t> Sum512(moku::slice<uint8_t> data)
{
    digest d{};

    d = digest{function : crypto::SHA512};
    d.Reset();
    d.Write(data);
    return d.checkSum();
}
std::vector<uint8_t> Sum384(moku::slice<uint8_t> data)
{
    digest d{};
    std::vector<uint8_t> sum{};
    std::vector<uint8_t> sum384{};

    d = digest{function : crypto::SHA384};
    d.Reset();
    d.Write(data);
    sum = d.checkSum();
    copy(moku::slice_expr<std::vector<uint8_t>>(sum384),
         moku::slice_expr<std::vector<uint8_t>>(sum, Size384));
    return;
}
std::vector<uint8_t> Sum512_224(moku::slice<uint8_t> data)
{
    digest d{};
    std::vector<uint8_t> sum{};
    std::vector<uint8_t> sum224{};

    d = digest{function : crypto::SHA512_224};
    d.Reset();
    d.Write(data);
    sum = d.checkSum();
    copy(moku::slice_expr<std::vector<uint8_t>>(sum224),
         moku::slice_expr<std::vector<uint8_t>>(sum, Size224));
    return;
}
std::vector<uint8_t> Sum512_256(moku::slice<uint8_t> data)
{
    digest d{};
    std::vector<uint8_t> sum{};
    std::vector<uint8_t> sum256{};

    d = digest{function : crypto::SHA512_256};
    d.Reset();
    d.Write(data);
    sum = d.checkSum();
    copy(moku::slice_expr<std::vector<uint8_t>>(sum256),
         moku::slice_expr<std::vector<uint8_t>>(sum, Size256));
    return;
}
void init();
int main()
{
    init();
    _K = moku::slice<uint64_t>{
            0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
            0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
            0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
            0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
            0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
            0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
            0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
            0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
            0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
            0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
            0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
            0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
            0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
            0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
            0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
            0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
            0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
            0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
            0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
            0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
            0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
            0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
            0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
            0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
            0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
            0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
            0x5fcb6fab3ad6faec, 0x6c44198c4a475817};
    useAVX2 = cpu::X86.HasAVX2 && cpu::X86.HasBMI1 && cpu::X86.HasBMI2;
    _main();
    return 0;
}
void blockGeneric(digest *dig, moku::slice<uint8_t> p)
{
    uint64_t h0{0};
    uint64_t h1{0};
    uint64_t h2{0};
    uint64_t h3{0};
    uint64_t h4{0};
    uint64_t h5{0};
    uint64_t h6{0};
    uint64_t h7{0};
    std::vector<uint64_t> w{};

    std::tie(h0, h1, h2, h3, h4, h5, h6, h7) = std::tuple<uint64_t,
                                                          uint64_t,
                                                          uint64_t,
                                                          uint64_t,
                                                          uint64_t,
                                                          uint64_t,
                                                          uint64_t,
                                                          uint64_t>(dig->h[0],
                                                                    dig->h[1],
                                                                    dig->h[2],
                                                                    dig->h[3],
                                                                    dig->h[4],
                                                                    dig->h[5],
                                                                    dig->h[6],
                                                                    dig->h[7]);
    while (len(p) >= chunk) {
        uint64_t a{0};
        uint64_t b{0};
        uint64_t c{0};
        uint64_t d{0};
        uint64_t e{0};
        uint64_t f{0};
        uint64_t g{0};
        uint64_t h{0};

        {
            int i{0};
            for (i = 0; i < 16; i++) {
                int j{0};

                j = i * 8;
                w[i] = uint64_t(p[j]) << 56 | uint64_t(p[j + 1]) << 48 |
                       uint64_t(p[j + 2]) << 40 | uint64_t(p[j + 3]) << 32 |
                       uint64_t(p[j + 4]) << 24 | uint64_t(p[j + 5]) << 16 |
                       uint64_t(p[j + 6]) << 8 | uint64_t(p[j + 7]);
            }
        }
        {
            int i{0};
            for (i = 16; i < 80; i++) {
                uint64_t t1{0};
                uint64_t t2{0};
                uint64_t v1{0};
                uint64_t v2{0};

                v1 = w[i - 2];
                t1 = (v1 >> 19 | v1 << (64 - 19)) ^
                     (v1 >> 61 | v1 << (64 - 61)) ^ (v1 >> 6);
                v2 = w[i - 15];
                t2 = (v2 >> 1 | v2 << (64 - 1)) ^ (v2 >> 8 | v2 << (64 - 8)) ^
                     (v2 >> 7);
                w[i] = t1 + w[i - 7] + t2 + w[i - 16];
            }
        }
        std::tie(a, b, c, d, e, f, g, h) =
                std::tuple<uint64_t,
                           uint64_t,
                           uint64_t,
                           uint64_t,
                           uint64_t,
                           uint64_t,
                           uint64_t,
                           uint64_t>(h0, h1, h2, h3, h4, h5, h6, h7);
        {
            int i{0};
            for (i = 0; i < 80; i++) {
                uint64_t t1{0};
                uint64_t t2{0};

                t1 = h +
                     ((e >> 14 | e << (64 - 14)) ^ (e >> 18 | e << (64 - 18)) ^
                      (e >> 41 | e << (64 - 41))) +
                     ((e & f) ^ (^e &g)) + _K[i] + w[i];
                t2 = ((a >> 28 | a << (64 - 28)) ^ (a >> 34 | a << (64 - 34)) ^
                      (a >> 39 | a << (64 - 39))) +
                     ((a & b) ^ (a & c) ^ (b & c));
                h = g;
                g = f;
                f = e;
                e = d + t1;
                d = c;
                c = b;
                b = a;
                a = t1 + t2;
            }
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
        p = moku::slice_expr<moku::slice<uint8_t>>(p, chunk);
    }
    std::tie(dig->h[0],
             dig->h[1],
             dig->h[2],
             dig->h[3],
             dig->h[4],
             dig->h[5],
             dig->h[6],
             dig->h[7]) = std::tuple<uint64_t,
                                     uint64_t,
                                     uint64_t,
                                     uint64_t,
                                     uint64_t,
                                     uint64_t,
                                     uint64_t,
                                     uint64_t>(h0, h1, h2, h3, h4, h5, h6, h7);
}
void init();
int main()
{
    init();
    _K = moku::slice<uint64_t>{
            0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
            0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
            0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
            0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
            0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
            0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
            0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
            0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
            0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
            0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
            0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
            0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
            0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
            0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
            0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
            0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
            0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
            0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
            0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
            0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
            0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
            0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
            0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
            0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
            0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
            0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
            0x5fcb6fab3ad6faec, 0x6c44198c4a475817};
    useAVX2 = cpu::X86.HasAVX2 && cpu::X86.HasBMI1 && cpu::X86.HasBMI2;
    _main();
    return 0;
}
void blockAVX2(digest *dig, moku::slice<uint8_t> p)
{
}
void blockAMD64(digest *dig, moku::slice<uint8_t> p)
{
}
void block(digest *dig, moku::slice<uint8_t> p)
{
    if (useAVX2) {
        blockAVX2(dig, p);
    } else {
        blockAMD64(dig, p);
    }
}
void init();
int main()
{
    init();
    _K = moku::slice<uint64_t>{
            0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
            0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
            0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
            0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
            0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
            0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
            0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
            0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
            0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
            0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
            0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
            0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
            0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
            0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
            0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
            0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
            0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
            0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
            0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
            0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
            0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
            0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
            0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
            0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
            0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
            0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
            0x5fcb6fab3ad6faec, 0x6c44198c4a475817};
    useAVX2 = cpu::X86.HasAVX2 && cpu::X86.HasBMI1 && cpu::X86.HasBMI2;
    _main();
    return 0;
}
