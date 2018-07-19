int ConstantTimeCompare(moku::slice<uint8_t> x, moku::slice<uint8_t> y)
{
    uint8_t v{0};

    if (len(x) != len(y)) {
        return 0;
    }
    {
        int i{0};
        for (i = 0; i < len(x); i++) {
            v |= x[i] ^ y[i];
        }
    }
    return ConstantTimeByteEq(v, 0);
}
int ConstantTimeSelect(int v, int x, int y)
{
    return ^(v - 1) & x | (v - 1) & y;
}
int ConstantTimeByteEq(uint8_t x, uint8_t y)
{
    uint8_t z{0};

    z = ^(x ^ y);
    z &= z >> 4;
    z &= z >> 2;
    z &= z >> 1;
    return int(z);
}
int ConstantTimeEq(int32_t x, int32_t y)
{
    int32_t z{0};

    z = ^(x ^ y);
    z &= z >> 16;
    z &= z >> 8;
    z &= z >> 4;
    z &= z >> 2;
    z &= z >> 1;
    return int(z & 1);
}
void ConstantTimeCopy(int v, moku::slice<uint8_t> x, moku::slice<uint8_t> y)
{
    uint8_t xmask{0};
    uint8_t ymask{0};

    if (len(x) != len(y)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 5596909) STRING
                     "subtle: slices have different lengths"}));
    }
    xmask = byte(v - 1);
    ymask = byte (^(v - 1));
    {
        int i{0};
        for (i = 0; i < len(x); i++) {
            x[i] = x[i] & xmask | y[i] & ymask;
        }
    }
}
int ConstantTimeLessOrEq(int x, int y)
{
    int32_t x32{0};
    int32_t y32{0};

    x32 = int32_t(x);
    y32 = int32_t(y);
    return int(((x32 - y32 - 1) >> 31) & 1);
}
int main()
{
    _main();
    return 0;
}
