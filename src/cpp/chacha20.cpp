void core(std::vector<uint8_t> *out,
          std::vector<uint8_t> *in,
          std::vector<uint8_t> *k)
{
    uint32_t j0{0};
    uint32_t j1{0};
    uint32_t j10{0};
    uint32_t j11{0};
    uint32_t j12{0};
    uint32_t j13{0};
    uint32_t j14{0};
    uint32_t j15{0};
    uint32_t j2{0};
    uint32_t j3{0};
    uint32_t j4{0};
    uint32_t j5{0};
    uint32_t j6{0};
    uint32_t j7{0};
    uint32_t j8{0};
    uint32_t j9{0};
    uint32_t x0{0};
    uint32_t x1{0};
    uint32_t x10{0};
    uint32_t x11{0};
    uint32_t x12{0};
    uint32_t x13{0};
    uint32_t x14{0};
    uint32_t x15{0};
    uint32_t x2{0};
    uint32_t x3{0};
    uint32_t x4{0};
    uint32_t x5{0};
    uint32_t x6{0};
    uint32_t x7{0};
    uint32_t x8{0};
    uint32_t x9{0};

    j0 = uint32_t(0x61707865);
    j1 = uint32_t(0x3320646e);
    j2 = uint32_t(0x79622d32);
    j3 = uint32_t(0x6b206574);
    j4 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 0, 4));
    j5 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 4, 8));
    j6 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 8, 12));
    j7 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 12, 16));
    j8 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 16, 20));
    j9 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 20, 24));
    j10 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 24, 28));
    j11 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(k, 28, 32));
    j12 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(in, 0, 4));
    j13 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(in, 4, 8));
    j14 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(in, 8, 12));
    j15 = binary::LittleEndian.Uint32(
            moku::slice_expr<std::vector<uint8_t> *>(in, 12, 16));
    std::tie(x0, x1, x2, x3, x4, x5, x6, x7) =
            std::tuple<uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t>(j0, j1, j2, j3, j4, j5, j6, j7);
    std::tie(x8, x9, x10, x11, x12, x13, x14, x15) =
            std::tuple<uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t,
                       uint32_t>(j8, j9, j10, j11, j12, j13, j14, j15);
    {
        int i{0};
        for (i = 0; i < rounds; i += 2) {
            x0 += x4;
            x12 ^= x0;
            x12 = (x12 << 16) | (x12 >> (16));
            x8 += x12;
            x4 ^= x8;
            x4 = (x4 << 12) | (x4 >> (20));
            x0 += x4;
            x12 ^= x0;
            x12 = (x12 << 8) | (x12 >> (24));
            x8 += x12;
            x4 ^= x8;
            x4 = (x4 << 7) | (x4 >> (25));
            x1 += x5;
            x13 ^= x1;
            x13 = (x13 << 16) | (x13 >> 16);
            x9 += x13;
            x5 ^= x9;
            x5 = (x5 << 12) | (x5 >> 20);
            x1 += x5;
            x13 ^= x1;
            x13 = (x13 << 8) | (x13 >> 24);
            x9 += x13;
            x5 ^= x9;
            x5 = (x5 << 7) | (x5 >> 25);
            x2 += x6;
            x14 ^= x2;
            x14 = (x14 << 16) | (x14 >> 16);
            x10 += x14;
            x6 ^= x10;
            x6 = (x6 << 12) | (x6 >> 20);
            x2 += x6;
            x14 ^= x2;
            x14 = (x14 << 8) | (x14 >> 24);
            x10 += x14;
            x6 ^= x10;
            x6 = (x6 << 7) | (x6 >> 25);
            x3 += x7;
            x15 ^= x3;
            x15 = (x15 << 16) | (x15 >> 16);
            x11 += x15;
            x7 ^= x11;
            x7 = (x7 << 12) | (x7 >> 20);
            x3 += x7;
            x15 ^= x3;
            x15 = (x15 << 8) | (x15 >> 24);
            x11 += x15;
            x7 ^= x11;
            x7 = (x7 << 7) | (x7 >> 25);
            x0 += x5;
            x15 ^= x0;
            x15 = (x15 << 16) | (x15 >> 16);
            x10 += x15;
            x5 ^= x10;
            x5 = (x5 << 12) | (x5 >> 20);
            x0 += x5;
            x15 ^= x0;
            x15 = (x15 << 8) | (x15 >> 24);
            x10 += x15;
            x5 ^= x10;
            x5 = (x5 << 7) | (x5 >> 25);
            x1 += x6;
            x12 ^= x1;
            x12 = (x12 << 16) | (x12 >> 16);
            x11 += x12;
            x6 ^= x11;
            x6 = (x6 << 12) | (x6 >> 20);
            x1 += x6;
            x12 ^= x1;
            x12 = (x12 << 8) | (x12 >> 24);
            x11 += x12;
            x6 ^= x11;
            x6 = (x6 << 7) | (x6 >> 25);
            x2 += x7;
            x13 ^= x2;
            x13 = (x13 << 16) | (x13 >> 16);
            x8 += x13;
            x7 ^= x8;
            x7 = (x7 << 12) | (x7 >> 20);
            x2 += x7;
            x13 ^= x2;
            x13 = (x13 << 8) | (x13 >> 24);
            x8 += x13;
            x7 ^= x8;
            x7 = (x7 << 7) | (x7 >> 25);
            x3 += x4;
            x14 ^= x3;
            x14 = (x14 << 16) | (x14 >> 16);
            x9 += x14;
            x4 ^= x9;
            x4 = (x4 << 12) | (x4 >> 20);
            x3 += x4;
            x14 ^= x3;
            x14 = (x14 << 8) | (x14 >> 24);
            x9 += x14;
            x4 ^= x9;
            x4 = (x4 << 7) | (x4 >> 25);
        }
    }
    x0 += j0;
    x1 += j1;
    x2 += j2;
    x3 += j3;
    x4 += j4;
    x5 += j5;
    x6 += j6;
    x7 += j7;
    x8 += j8;
    x9 += j9;
    x10 += j10;
    x11 += j11;
    x12 += j12;
    x13 += j13;
    x14 += j14;
    x15 += j15;
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 0, 4), x0);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 4, 8), x1);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 8, 12), x2);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 12, 16), x3);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 16, 20), x4);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 20, 24), x5);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 24, 28), x6);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 28, 32), x7);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 32, 36), x8);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 36, 40), x9);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 40, 44), x10);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 44, 48), x11);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 48, 52), x12);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 52, 56), x13);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 56, 60), x14);
    binary::LittleEndian.PutUint32(
            moku::slice_expr<std::vector<uint8_t> *>(out, 60, 64), x15);
}
void XORKeyStream(moku::slice<uint8_t> out,
                  moku::slice<uint8_t> in,
                  std::vector<uint8_t> *counter,
                  std::vector<uint8_t> *key)
{
    std::vector<uint8_t> block{};
    std::vector<uint8_t> counterCopy{};

    copy(moku::slice_expr<std::vector<uint8_t>>(counterCopy),
         moku::slice_expr<std::vector<uint8_t> *>(counter));
    while (len(in) >= 64) {
        uint32_t u{0};

        core(&block, &counterCopy, key);
        {
            int i{0};
            uint8_t x{0};
            for (std::tie(i, x) :
                 moku::range_key_value<std::vector<uint8_t>>(block)) {
                out[i] = in[i] ^ x;
            }
        }
        u = uint32_t(1);
        {
            int i{0};
            for (i = 0; i < 4; i++) {
                u += uint32_t(counterCopy[i]);
                counterCopy[i] = byte(u);
                u >>= 8;
            }
        }
        in = moku::slice_expr<moku::slice<uint8_t>>(in, 64);
        out = moku::slice_expr<moku::slice<uint8_t>>(out, 64);
    }
    if (len(in) > 0) {
        core(&block, &counterCopy, key);
        {
            int i{0};
            uint8_t v{0};
            for (std::tie(i, v) :
                 moku::range_key_value<moku::slice<uint8_t>>(in)) {
                out[i] = v ^ block[i];
            }
        }
    }
}
int main()
{
    _main();
    return 0;
}
