std::tuple<bool, moku::error> ParseBool(std::string str)
{
    if ((str == "1") || (str == "t") || (str == "T") || (str == "true") ||
        (str == "TRUE") || (str == "True")) {
    _ident_0_:
        return {true, nil};
    } else if ((str == "0") || (str == "f") || (str == "F") ||
               (str == "false") || (str == "FALSE") || (str == "False")) {
    _ident_1_:
        return {false, nil};
    }
    return {false, syntaxError("ParseBool", str)};
}
std::string FormatBool(bool b)
{
    if (b) {
        return "true";
    }
    return "false";
}
moku::slice<uint8_t> AppendBool(moku::slice<uint8_t> dst, bool b)
{
    if (b) {
        return append(dst, "true");
    }
    return append(dst, "false");
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
bool equalIgnoreCase(std::string s1, std::string s2)
{
    if (len(s1) != len(s2)) {
        return false;
    }
    {
        int i{0};
        for (i = 0; i < len(s1); i++) {
            uint8_t c1{0};
            uint8_t c2{0};

            c1 = s1[i];
            if ('A' <= c1 && c1 <= 'Z') {
                c1 += 'a' - 'A';
            }
            c2 = s2[i];
            if ('A' <= c2 && c2 <= 'Z') {
                c2 += 'a' - 'A';
            }
            if (c1 != c2) {
                return false;
            }
        }
    }
    return true;
}
std::tuple<double, bool> special(std::string s)
{
    double f{0};
    bool ok{false};

    if (len(s) == 0) {
        return;
    }
    if ((s[0] == '+')) {
    _ident_1_:
        if (equalIgnoreCase(s, "+inf") || equalIgnoreCase(s, "+infinity")) {
            return {math::Inf(1), true};
        }
    } else if ((s[0] == '-')) {
    _ident_2_:
        if (equalIgnoreCase(s, "-inf") || equalIgnoreCase(s, "-infinity")) {
            return {math::Inf(-1), true};
        }
    } else if ((s[0] == 'n') || (s[0] == 'N')) {
    _ident_3_:
        if (equalIgnoreCase(s, "nan")) {
            return {math::NaN(), true};
        }
    } else if ((s[0] == 'i') || (s[0] == 'I')) {
    _ident_4_:
        if (equalIgnoreCase(s, "inf") || equalIgnoreCase(s, "infinity")) {
            return {math::Inf(1), true};
        }
    } else {
    _ident_0_:
        return;
    }
    return;
}
bool decimal::set(std::string s)
{
    int i{0};
    bool ok{false};
    bool sawdigits{false};
    bool sawdot{false};

    i = 0;
    this->neg = false;
    this->trunc = false;
    if (i >= len(s)) {
        return;
    }
    if ((s[i] == '+')) {
    _ident_5_:
        i++;
    } else if ((s[i] == '-')) {
    _ident_6_:
        this->neg = true;
        i++;
    }
    sawdot = false;
    sawdigits = false;
    for (; i < len(s); i++) {
        if ((s[i] == '.')) {
        _ident_7_:
            if (sawdot) {
                return;
            }
            sawdot = true;
            this->dp = this->nd;
            continue;
        } else if (('0' <= s[i] && s[i] <= '9')) {
        _ident_8_:
            sawdigits = true;
            if (s[i] == '0' && this->nd == 0) {
                this->dp--;
                continue;
            }
            if (this->nd < len(this->d)) {
                this->d[this->nd] = s[i];
                this->nd++;
            } else {
                if (s[i] != '0') {
                    this->trunc = true;
                }
            }
            continue;
        }
        break;
    }
    if (!sawdigits) {
        return;
    }
    if (!sawdot) {
        this->dp = this->nd;
    }
    if (i < len(s) && (s[i] == 'e' || s[i] == 'E')) {
        i++;
        if (i >= len(s)) {
            return;
        }
        esign = 1;
        if (s[i] == '+') {
            i++;
        } else {
            if (s[i] == '-') {
                i++;
                esign = -1;
            }
        }
        if (i >= len(s) || s[i] < '0' || s[i] > '9') {
            return;
        }
        e = 0;
        for (; i < len(s) && '0' <= s[i] && s[i] <= '9'; i++) {
            if (e < 10000) {
                e = e * 10 + int(s[i]) - '0';
            }
        }
        this->dp += e * esign;
    }
    if (i != len(s)) {
        return;
    }
    ok = true;
    return;
}
std::tuple<uint64_t, int, bool, bool, bool> readFloat(std::string s)
{
    int dp{0};
    int exp{0};
    int i{0};
    uint64_t mantissa{0};
    int nd{0};
    int ndMant{0};
    bool neg{false};
    bool ok{false};
    bool sawdigits{false};
    bool sawdot{false};
    bool trunc{false};
    constexpr int uint64digits{19};

    i = 0;
    if (i >= len(s)) {
        return;
    }
    if ((s[i] == '+')) {
    _ident_9_:
        i++;
    } else if ((s[i] == '-')) {
    _ident_10_:
        neg = true;
        i++;
    }
    sawdot = false;
    sawdigits = false;
    nd = 0;
    ndMant = 0;
    dp = 0;
    for (; i < len(s); i++) {
        {
            uint8_t c{0};
            c = s[i];
            if ((true == c == '.')) {
            _ident_11_:
                if (sawdot) {
                    return;
                }
                sawdot = true;
                dp = nd;
                continue;
            } else if ((true == '0' <= c && c <= '9')) {
            _ident_12_:
                sawdigits = true;
                if (c == '0' && nd == 0) {
                    dp--;
                    continue;
                }
                nd++;
                if (ndMant < uint64digits) {
                    mantissa *= 10;
                    mantissa += uint64_t(c - '0');
                    ndMant++;
                } else {
                    if (s[i] != '0') {
                        trunc = true;
                    }
                }
                continue;
            }
        }
        break;
    }
    if (!sawdigits) {
        return;
    }
    if (!sawdot) {
        dp = nd;
    }
    if (i < len(s) && (s[i] == 'e' || s[i] == 'E')) {
        i++;
        if (i >= len(s)) {
            return;
        }
        esign = 1;
        if (s[i] == '+') {
            i++;
        } else {
            if (s[i] == '-') {
                i++;
                esign = -1;
            }
        }
        if (i >= len(s) || s[i] < '0' || s[i] > '9') {
            return;
        }
        e = 0;
        for (; i < len(s) && '0' <= s[i] && s[i] <= '9'; i++) {
            if (e < 10000) {
                e = e * 10 + int(s[i]) - '0';
            }
        }
        dp += e * esign;
    }
    if (i != len(s)) {
        return;
    }
    if (mantissa != 0) {
        exp = dp - ndMant;
    }
    ok = true;
    return;
}
std::tuple<uint64_t, bool> decimal::floatBits(floatInfo *flt)
{
    uint64_t b{0};
    uint64_t bits{0};
    int exp{0};
    uint64_t mant{0};
    bool overflow{false};

    if (this->nd == 0) {
        mant = 0;
        exp = flt->bias;
        goto out;
    }
    if (this->dp > 310) {
        goto overflow;
    }
    if (this->dp < -330) {
        mant = 0;
        exp = flt->bias;
        goto out;
    }
    exp = 0;
    while (this->dp > 0) {
        int n{0};

        if (this->dp >= len(powtab)) {
            n = 27;
        } else {
            n = powtab[this->dp];
        }
        this->Shift(-n);
        exp += n;
    }
    while (this->dp < 0 || this->dp == 0 && this->d[0] < '5') {
        int n{0};

        if (-this->dp >= len(powtab)) {
            n = 27;
        } else {
            n = powtab[-this->dp];
        }
        this->Shift(n);
        exp -= n;
    }
    exp--;
    if (exp < flt->bias + 1) {
        n = flt->bias + 1 - exp;
        this->Shift(-n);
        exp += n;
    }
    if (exp - flt->bias >= 1 << flt->expbits - 1) {
        goto overflow;
    }
    this->Shift(int(1 + flt->mantbits));
    mant = this->RoundedInteger();
    if (mant == 2 << flt->mantbits) {
        mant >>= 1;
        exp++;
        if (exp - flt->bias >= 1 << flt->expbits - 1) {
            goto overflow;
        }
    }
    if (mant & (1 << flt->mantbits) == 0) {
        exp = flt->bias;
    }
    goto out;
overflow:;
    exp = 1 << flt->expbits - 1 + flt->bias;
    overflow = true;
out:;
    bits |= uint64_t((exp - flt->bias) & (1 << flt->expbits - 1))
            << flt->mantbits;
    if (this->neg) {
        bits |= 1 << flt->mantbits << flt->expbits;
    }
    return {bits, overflow};
}
std::tuple<double, bool> atof64exact(uint64_t mantissa, int exp, bool neg)
{
    double f{0};
    bool ok{false};

    if (mantissa >> float64info.mantbits != 0) {
        return;
    }
    f = double(mantissa);
    if (neg) {
        f = -f;
    }
    if ((exp == 0)) {
    _ident_13_:
        return {f, true};
    } else if ((exp > 0 && exp <= 15 + 22)) {
    _ident_14_:
        if (exp > 22) {
            f *= float64pow10[exp - 22];
            exp = 22;
        }
        if (f > 1e15 || f < -1e15) {
            return;
        }
        return {f * float64pow10[exp], true};
    } else if ((exp < 0 && exp >= -22)) {
    _ident_15_:
        return {f / float64pow10[-exp], true};
    }
    return;
}
std::tuple<float, bool> atof32exact(uint64_t mantissa, int exp, bool neg)
{
    float f{0};
    bool ok{false};

    if (mantissa >> float32info.mantbits != 0) {
        return;
    }
    f = float(mantissa);
    if (neg) {
        f = -f;
    }
    if ((exp == 0)) {
    _ident_16_:
        return {f, true};
    } else if ((exp > 0 && exp <= 7 + 10)) {
    _ident_17_:
        if (exp > 10) {
            f *= float32pow10[exp - 10];
            exp = 10;
        }
        if (f > 1e7 || f < -1e7) {
            return;
        }
        return {f * float32pow10[exp], true};
    } else if ((exp < 0 && exp >= -10)) {
    _ident_18_:
        return {f / float32pow10[-exp], true};
    }
    return;
}
std::tuple<float, moku::error> atof32(std::string s)
{
    uint64_t b{0};
    decimal d{};
    moku::error err{};
    float f{0};
    bool ovf{false};

    {
        bool ok{false};
        double val{0};

        std::tie(val, ok) = special(s);

        if (ok) {
            return {float(val), nil};
        }
    }
    if (optimize) {
        std::tie(mantissa, exp, neg, trunc, ok) = readFloat(s);
        if (ok) {
            if (!trunc) {
                {
                    float f{0};
                    bool ok{false};

                    std::tie(f, ok) = atof32exact(mantissa, exp, neg);

                    if (ok) {
                        return {f, nil};
                    }
                }
            }
            ext = new (extFloat);
            {
                bool ok{false};

                ok = ext->AssignDecimal(
                        mantissa, exp, neg, trunc, &float32info);

                if (ok) {
                    std::tie(b, ovf) = ext->floatBits(&float32info);
                    f = math::Float32frombits(uint32_t(b));
                    if (ovf) {
                        err = rangeError(fnParseFloat, s);
                    }
                    return {f, err};
                }
            }
        }
    }
    if (!d.set(s)) {
        return {0, syntaxError(fnParseFloat, s)};
    }
    std::tie(b, ovf) = d.floatBits(&float32info);
    f = math::Float32frombits(uint32_t(b));
    if (ovf) {
        err = rangeError(fnParseFloat, s);
    }
    return {f, err};
}
std::tuple<double, moku::error> atof64(std::string s)
{
    uint64_t b{0};
    decimal d{};
    moku::error err{};
    double f{0};
    bool ovf{false};

    {
        bool ok{false};
        double val{0};

        std::tie(val, ok) = special(s);

        if (ok) {
            return {val, nil};
        }
    }
    if (optimize) {
        std::tie(mantissa, exp, neg, trunc, ok) = readFloat(s);
        if (ok) {
            if (!trunc) {
                {
                    double f{0};
                    bool ok{false};

                    std::tie(f, ok) = atof64exact(mantissa, exp, neg);

                    if (ok) {
                        return {f, nil};
                    }
                }
            }
            ext = new (extFloat);
            {
                bool ok{false};

                ok = ext->AssignDecimal(
                        mantissa, exp, neg, trunc, &float64info);

                if (ok) {
                    std::tie(b, ovf) = ext->floatBits(&float64info);
                    f = math::Float64frombits(b);
                    if (ovf) {
                        err = rangeError(fnParseFloat, s);
                    }
                    return {f, err};
                }
            }
        }
    }
    if (!d.set(s)) {
        return {0, syntaxError(fnParseFloat, s)};
    }
    std::tie(b, ovf) = d.floatBits(&float64info);
    f = math::Float64frombits(b);
    if (ovf) {
        err = rangeError(fnParseFloat, s);
    }
    return {f, err};
}
std::tuple<double, moku::error> ParseFloat(std::string s, int bitSize)
{
    if (bitSize == 32) {
        std::tie(f, err) = atof32(s);
        return {double(f), err};
    }
    return atof64(s);
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
std::string NumError::Error()
{
    return "strconv." + this->Func + ": " + "parsing " + Quote(this->Num) +
           ": " + this->Err.Error();
}
NumError *syntaxError(std::string fn, std::string str)
{
    return &NumError{fn, str, ErrSyntax};
}
NumError *rangeError(std::string fn, std::string str)
{
    return &NumError{fn, str, ErrRange};
}
std::tuple<uint64_t, moku::error> ParseUint(std::string s,
                                            int base,
                                            int bitSize)
{
    uint64_t cutoff{0};
    moku::error err{};
    int i{0};
    uint64_t maxVal{0};
    uint64_t n{0};

    if (bitSize == 0) {
        bitSize = int(IntSize);
    }
    i = 0;
    if ((len(s) < 1)) {
    _ident_0_:
        err = ErrSyntax;
        goto Error;
    } else if ((2 <= base && base <= 36)) {
    _ident_1_:
    } else if ((base == 0)) {
    _ident_2_:
        if ((s[0] == '0' && len(s) > 1 && (s[1] == 'x' || s[1] == 'X'))) {
        _ident_4_:
            if (len(s) < 3) {
                err = ErrSyntax;
                goto Error;
            }
            base = 16;
            i = 2;
        } else if ((s[0] == '0')) {
        _ident_5_:
            base = 8;
            i = 1;
        } else {
        _ident_6_:
            base = 10;
        }
    } else {
    _ident_2_:
        err = errors::New("invalid base " + Itoa(base));
        goto Error;
    }
    if ((base == 10)) {
    _ident_7_:
        cutoff = maxUint64 / 10 + 1;
    } else if ((base == 16)) {
    _ident_8_:
        cutoff = maxUint64 / 16 + 1;
    } else {
    _ident_9_:
        cutoff = maxUint64 / uint64_t(base) + 1;
    }
    maxVal = 1 << unsigned int(bitSize) - 1;
    for (; i < len(s); i++) {
        uint8_t d{0};
        uint64_t n1{0};
        uint8_t v{0};

        d = s[i];
        if (('0' <= d && d <= '9')) {
        _ident_10_:
            v = d - '0';
        } else if (('a' <= d && d <= 'z')) {
        _ident_11_:
            v = d - 'a' + 10;
        } else if (('A' <= d && d <= 'Z')) {
        _ident_12_:
            v = d - 'A' + 10;
        } else {
        _ident_13_:
            n = 0;
            err = ErrSyntax;
            goto Error;
        }
        if (v >= byte(base)) {
            n = 0;
            err = ErrSyntax;
            goto Error;
        }
        if (n >= cutoff) {
            n = maxUint64;
            err = ErrRange;
            goto Error;
        }
        n *= uint64_t(base);
        n1 = n + uint64_t(v);
        if (n1 < n || n1 > maxVal) {
            n = maxUint64;
            err = ErrRange;
            goto Error;
        }
        n = n1;
    }
    return {n, nil};
Error:;
}
std::tuple<int64_t, moku::error> ParseInt(std::string s, int base, int bitSize)
{
    uint64_t cutoff{0};
    moku::error err{};
    constexpr std::string fnParseInt{"ParseInt"};
    int64_t i{0};
    int64_t n{0};
    bool neg{false};
    std::string s0{""};
    uint64_t un{0};

    if (bitSize == 0) {
        bitSize = int(IntSize);
    }
    if (len(s) == 0) {
        return {0, syntaxError(fnParseInt, s)};
    }
    s0 = s;
    neg = false;
    if (s[0] == '+') {
        s = moku::slice_expr<std::string>(s, 1);
    } else {
        if (s[0] == '-') {
            neg = true;
            s = moku::slice_expr<std::string>(s, 1);
        }
    }
    std::tie(un, err) = ParseUint(s, base, bitSize);
    if (!moku::is_nil(err) &&
        moku::try_assert<*NumError>(err)->Err != ErrRange) {
        moku::try_assert<*NumError>(err)->Func = fnParseInt;
        moku::type_assert<*NumError>(err)->Num = s0;
        return {0, err};
    }
    cutoff = uint64_t(1 << unsigned int(bitSize - 1));
    if (!neg && un >= cutoff) {
        return {int64_t(cutoff - 1), rangeError(fnParseInt, s0)};
    }
    if (neg && un > cutoff) {
        return {-int64_t(cutoff), rangeError(fnParseInt, s0)};
    }
    n = int64_t(un);
    if (neg) {
        n = -n;
    }
    return {n, nil};
}
std::tuple<int, moku::error> Atoi(std::string s)
{
    moku::error err{};
    constexpr std::string fnAtoi{"Atoi"};
    int64_t i64{0};

    std::tie(i64, err) = ParseInt(s, 10, 0);
    {
        NumError *nerr{std::nullptr};
        bool ok{false};

        std::tie(nerr, ok) = moku::try_assert<*NumError>(err);

        if (ok) {
            nerr->Func = fnAtoi;
        }
    }
    return {int(i64), err};
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
std::string decimal::String()
{
    moku::slice<uint8_t> buf{};
    int n{0};
    int w{0};

    n = 10 + this->nd;
    if (this->dp > 0) {
        n += this->dp;
    }
    if (this->dp < 0) {
        n += -this->dp;
    }
    buf = make(moku::slice<byte>, n);
    w = 0;
    if ((this->nd == 0)) {
    _ident_0_:
        return "0";
    } else if ((this->dp <= 0)) {
    _ident_1_:
        buf[w] = '0';
        w++;
        buf[w] = '.';
        w++;
        w += digitZero(
                moku::slice_expr<moku::slice<uint8_t>>(buf, w, w + -this->dp));
        w += copy(moku::slice_expr<moku::slice<uint8_t>>(buf, w),
                  moku::slice_expr<std::vector<uint8_t>>(this->d, 0, this->nd));
    } else if ((this->dp < this->nd)) {
    _ident_2_:
        w += copy(moku::slice_expr<moku::slice<uint8_t>>(buf, w),
                  moku::slice_expr<std::vector<uint8_t>>(this->d, 0, this->dp));
        buf[w] = '.';
        w++;
        w += copy(moku::slice_expr<moku::slice<uint8_t>>(buf, w),
                  moku::slice_expr<std::vector<uint8_t>>(
                          this->d, this->dp, this->nd));
    } else {
    _ident_3_:
        w += copy(moku::slice_expr<moku::slice<uint8_t>>(buf, w),
                  moku::slice_expr<std::vector<uint8_t>>(this->d, 0, this->nd));
        w += digitZero(moku::slice_expr<moku::slice<uint8_t>>(
                buf, w, w + this->dp - this->nd));
    }
    return std::string(moku::slice_expr<moku::slice<uint8_t>>(buf, 0, w));
}
int digitZero(moku::slice<uint8_t> dst)
{
    {
        int i{0};
        for (i : moku::range_key<moku::slice<uint8_t>>(dst)) {
            dst[i] = '0';
        }
    }
    return len(dst);
}
void trim(decimal *a)
{
    while (a->nd > 0 && a->d[a->nd - 1] == '0') {
        a->nd--;
    }
    if (a->nd == 0) {
        a->dp = 0;
    }
}
void decimal::Assign(uint64_t v)
{
    std::vector<uint8_t> buf{};
    int n{0};

    n = 0;
    while (v > 0) {
        uint64_t v1{0};

        v1 = v / 10;
        v -= 10 * v1;
        buf[n] = byte(v + '0');
        n++;
        v = v1;
    }
    this->nd = 0;
    for (n--; n >= 0; n--) {
        this->d[this->nd] = buf[n];
        this->nd++;
    }
    this->dp = this->nd;
    trim(this);
}
void rightShift(decimal *a, unsigned int k)
{
    unsigned int mask{0};
    unsigned int n{0};
    int r{0};
    int w{0};

    r = 0;
    w = 0;
    for (; n >> k == 0; r++) {
        unsigned int c{0};

        if (r >= a->nd) {
            if (n == 0) {
                a->nd = 0;
                return;
            }
            while (n >> k == 0) {
                n = n * 10;
                r++;
            }
            break;
        }
        c = unsigned int(a->d[r]);
        n = n * 10 + c - '0';
    }
    a->dp -= r - 1;
    for (; r < a->nd; r++) {
        unsigned int c{0};
        unsigned int dig{0};

        c = unsigned int(a->d[r]);
        dig = n >> k;
        n &= mask;
        a->d[w] = byte(dig + '0');
        w++;
        n = n * 10 + c - '0';
    }
    while (n > 0) {
        unsigned int dig{0};

        dig = n >> k;
        n &= mask;
        if (w < len(a->d)) {
            a->d[w] = byte(dig + '0');
            w++;
        } else {
            if (dig > 0) {
                a->trunc = true;
            }
        }
        n = n * 10;
    }
    a->nd = w;
    trim(a);
}
bool prefixIsLessThan(moku::slice<uint8_t> b, std::string s)
{
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            if (i >= len(b)) {
                return true;
            }
            if (b[i] != s[i]) {
                return b[i] < s[i];
            }
        }
    }
    return false;
}
void leftShift(decimal *a, unsigned int k)
{
    int delta{0};
    unsigned int n{0};
    int r{0};
    int w{0};

    delta = leftcheats[k].delta;
    if (prefixIsLessThan(moku::slice_expr<std::vector<uint8_t>>(a->d, 0, a->nd),
                         leftcheats[k].cutoff)) {
        delta--;
    }
    r = a->nd;
    w = a->nd + delta;
    for (r--; r >= 0; r--) {
        unsigned int quo{0};
        unsigned int rem{0};

        n += (unsigned int(a->d[r]) - '0') << k;
        quo = n / 10;
        rem = n - 10 * quo;
        w--;
        if (w < len(a->d)) {
            a->d[w] = byte(rem + '0');
        } else {
            if (rem != 0) {
                a->trunc = true;
            }
        }
        n = quo;
    }
    while (n > 0) {
        unsigned int quo{0};
        unsigned int rem{0};

        quo = n / 10;
        rem = n - 10 * quo;
        w--;
        if (w < len(a->d)) {
            a->d[w] = byte(rem + '0');
        } else {
            if (rem != 0) {
                a->trunc = true;
            }
        }
        n = quo;
    }
    a->nd += delta;
    if (a->nd >= len(a->d)) {
        a->nd = len(a->d);
    }
    a->dp += delta;
    trim(a);
}
void decimal::Shift(int k)
{
    if ((this->nd == 0)) {
    _ident_4_:
    } else if ((k > 0)) {
    _ident_5_:
        while (k > maxShift) {
            leftShift(this, maxShift);
            k -= maxShift;
        }
        leftShift(this, unsigned int(k));
    } else if ((k < 0)) {
    _ident_6_:
        while (k < -maxShift) {
            rightShift(this, maxShift);
            k += maxShift;
        }
        rightShift(this, unsigned int(-k));
    }
}
bool shouldRoundUp(decimal *a, int nd)
{
    if (nd < 0 || nd >= a->nd) {
        return false;
    }
    if (a->d[nd] == '5' && nd + 1 == a->nd) {
        if (a->trunc) {
            return true;
        }
        return nd > 0 && (a->d[nd - 1] - '0') % 2 != 0;
    }
    return a->d[nd] >= '5';
}
void decimal::Round(int nd)
{
    if (nd < 0 || nd >= this->nd) {
        return;
    }
    if (shouldRoundUp(this, nd)) {
        this->RoundUp(nd);
    } else {
        this->RoundDown(nd);
    }
}
void decimal::RoundDown(int nd)
{
    if (nd < 0 || nd >= this->nd) {
        return;
    }
    this->nd = nd;
    trim(this);
}
void decimal::RoundUp(int nd)
{
    if (nd < 0 || nd >= this->nd) {
        return;
    }
    {
        int i{0};
        for (i = nd - 1; i >= 0; i--) {
            uint8_t c{0};

            c = this->d[i];
            if (c < '9') {
                this->d[i]++;
                this->nd = i + 1;
                return;
            }
        }
    }
    this->d[0] = '1';
    this->nd = 1;
    this->dp++;
}
uint64_t decimal::RoundedInteger()
{
    int i{0};
    uint64_t n{0};

    if (this->dp > 20) {
        return 0xFFFFFFFFFFFFFFFF;
    }
    n = uint64_t(0);
    for (i = 0; i < this->dp && i < this->nd; i++) {
        n = n * 10 + uint64_t(this->d[i] - '0');
    }
    for (; i < this->dp; i++) {
        n *= 10;
    }
    if (shouldRoundUp(this, this->dp)) {
        n++;
    }
    return n;
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
std::tuple<uint64_t, bool> extFloat::floatBits(floatInfo *flt)
{
    uint64_t bits{0};
    int exp{0};
    uint64_t mant{0};
    bool overflow{false};

    this->Normalize();
    exp = this->exp + 63;
    if (exp < flt->bias + 1) {
        n = flt->bias + 1 - exp;
        this->mant >>= unsigned int(n);
        exp += n;
    }
    mant = this->mant >> (63 - flt->mantbits);
    if (this->mant & (1 << (62 - flt->mantbits)) != 0) {
        mant += 1;
    }
    if (mant == 2 << flt->mantbits) {
        mant >>= 1;
        exp++;
    }
    if (exp - flt->bias >= 1 << flt->expbits - 1) {
        mant = 0;
        exp = 1 << flt->expbits - 1 + flt->bias;
        overflow = true;
    } else {
        if (mant & (1 << flt->mantbits) == 0) {
            exp = flt->bias;
        }
    }
    bits = mant & (uint64_t(1) << flt->mantbits - 1);
    bits |= uint64_t((exp - flt->bias) & (1 << flt->expbits - 1))
            << flt->mantbits;
    if (this->neg) {
        bits |= 1 << (flt->mantbits + flt->expbits);
    }
    return;
}
std::tuple<extFloat, extFloat> extFloat::AssignComputeBounds(uint64_t mant,
                                                             int exp,
                                                             bool neg,
                                                             floatInfo *flt)
{
    int expBiased{0};
    extFloat lower{};
    extFloat upper{};

    this->mant = mant;
    this->exp = exp - int(flt->mantbits);
    this->neg = neg;
    if (this->exp <= 0 && mant == (mant >> unsigned int(-this->exp))
                                          << unsigned int(-this->exp)) {
        this->mant >>= unsigned int(-this->exp);
        this->exp = 0;
        return {*this, *this};
    }
    expBiased = exp - flt->bias;
    upper =
    extFloat{mant : 2 * this->mant + 1, exp : this->exp - 1, neg : this->neg};
    if (mant != 1 << flt->mantbits || expBiased == 1) {
        lower = extFloat{
            mant : 2 * this->mant - 1,
            exp : this->exp - 1,
            neg : this->neg
        };
    } else {
        lower = extFloat{
            mant : 4 * this->mant - 1,
            exp : this->exp - 2,
            neg : this->neg
        };
    }
    return;
}
unsigned int extFloat::Normalize()
{
    int exp{0};
    uint64_t mant{0};
    unsigned int shift{0};

    std::tie(mant, exp) = std::tuple<uint64_t, int>(this->mant, this->exp);
    if (mant == 0) {
        return 0;
    }
    if (mant >> (64 - 32) == 0) {
        mant <<= 32;
        exp -= 32;
    }
    if (mant >> (64 - 16) == 0) {
        mant <<= 16;
        exp -= 16;
    }
    if (mant >> (64 - 8) == 0) {
        mant <<= 8;
        exp -= 8;
    }
    if (mant >> (64 - 4) == 0) {
        mant <<= 4;
        exp -= 4;
    }
    if (mant >> (64 - 2) == 0) {
        mant <<= 2;
        exp -= 2;
    }
    if (mant >> (64 - 1) == 0) {
        mant <<= 1;
        exp -= 1;
    }
    shift = unsigned int(this->exp - exp);
    std::tie(this->mant, this->exp) = std::tuple<uint64_t, int>(mant, exp);
    return;
}
void extFloat::Multiply(extFloat g)
{
    uint64_t cross1{0};
    uint64_t cross2{0};
    uint64_t fhi{0};
    uint64_t flo{0};
    uint64_t ghi{0};
    uint64_t glo{0};
    uint64_t rem{0};

    std::tie(fhi, flo) = std::tuple<uint64_t, uint64_t>(
            this->mant >> 32, uint64_t(uint32_t(this->mant)));
    std::tie(ghi, glo) = std::tuple<uint64_t, uint64_t>(
            g.mant >> 32, uint64_t(uint32_t(g.mant)));
    cross1 = fhi * glo;
    cross2 = flo * ghi;
    this->mant = fhi * ghi + (cross1 >> 32) + (cross2 >> 32);
    rem = uint64_t(uint32_t(cross1)) + uint64_t(uint32_t(cross2)) +
          ((flo * glo) >> 32);
    rem += (1 << 31);
    this->mant += (rem >> 32);
    this->exp = this->exp + g.exp + 64;
}
bool extFloat::AssignDecimal(
        uint64_t mantissa, int exp10, bool neg, bool trunc, floatInfo *flt)
{
    int adjExp{0};
    int denormalExp{0};
    int errors{0};
    constexpr int errorscale{8};
    unsigned int extrabits{0};
    uint64_t halfway{0};
    int i{0};
    uint64_t mant_extra{0};
    bool ok{false};
    unsigned int shift{0};
    constexpr int uint64digits{19};

    errors = 0;
    if (trunc) {
        errors += errorscale / 2;
    }
    this->mant = mantissa;
    this->exp = 0;
    this->neg = neg;
    i = (exp10 - firstPowerOfTen) / stepPowerOfTen;
    if (exp10 < firstPowerOfTen || i >= len(powersOfTen)) {
        return false;
    }
    adjExp = (exp10 - firstPowerOfTen) % stepPowerOfTen;
    if (adjExp < uint64digits &&
        mantissa < uint64pow10[uint64digits - adjExp]) {
        this->mant *= uint64pow10[adjExp];
        this->Normalize();
    } else {
        this->Normalize();
        this->Multiply(smallPowersOfTen[adjExp]);
        errors += errorscale / 2;
    }
    this->Multiply(powersOfTen[i]);
    if (errors > 0) {
        errors += 1;
    }
    errors += errorscale / 2;
    shift = this->Normalize();
    errors <<= shift;
    denormalExp = flt->bias - 63;
    if (this->exp <= denormalExp) {
        extrabits =
                63 - flt->mantbits + 1 + unsigned int(denormalExp - this->exp);
    } else {
        extrabits = 63 - flt->mantbits;
    }
    halfway = uint64_t(1) << (extrabits - 1);
    mant_extra = this->mant & (1 << extrabits - 1);
    if (int64_t(halfway) - int64_t(errors) < int64_t(mant_extra) &&
        int64_t(mant_extra) < int64_t(halfway) + int64_t(errors)) {
        return false;
    }
    return true;
}
std::tuple<int, int> extFloat::frexp10()
{
    int approxExp10{0};
    int exp10{0};
    constexpr int expMax{-32};
    constexpr int expMin{-60};
    int i{0};
    int index{0};

    approxExp10 = ((expMin + expMax) / 2 - this->exp) * 28 / 93;
    i = (approxExp10 - firstPowerOfTen) / stepPowerOfTen;
Loop:;
    this->Multiply(powersOfTen[i]);
    return {-(firstPowerOfTen + i * stepPowerOfTen), i};
}
int frexp10Many(extFloat *a, extFloat *b, extFloat *c)
{
    int exp10{0};
    int i{0};

    std::tie(exp10, i) = c->frexp10();
    a->Multiply(powersOfTen[i]);
    b->Multiply(powersOfTen[i]);
    return;
}
bool extFloat::FixedDecimal(decimalSlice *d, int n)
{
    std::vector<uint8_t> buf{};
    int exp10{0};
    uint64_t fraction{0};
    uint32_t integer{0};
    int integerDigits{0};
    int nd{0};
    int needed{0};
    bool ok{false};
    int pos{0};
    uint64_t pow10{0};
    uint32_t rest{0};
    unsigned int shift{0};
    uint64_t ε{0};

    if (this->mant == 0) {
        d->nd = 0;
        d->dp = 0;
        d->neg = this->neg;
        return true;
    }
    if (n == 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 1383787) STRING "strconv: internal error: "
                                                    "extFloat.FixedDecimal "
                                                    "called with n == 0"}));
    }
    this->Normalize();
    std::tie(exp10, _) = this->frexp10();
    shift = unsigned int(-this->exp);
    integer = uint32_t(this->mant >> shift);
    fraction = this->mant - (uint64_t(integer) << shift);
    ε = uint64_t(1);
    needed = n;
    integerDigits = 0;
    pow10 = uint64_t(1);
    {
        int i{0};
        uint64_t pow{0};
        for (std::tie(i, pow) = std::tuple<int, uint64_t>(0, uint64_t(1));
             i < 20;
             i++) {
            if (pow > uint64_t(integer)) {
                integerDigits = i;
                break;
            }
            pow *= 10;
        }
    }
    rest = integer;
    if (integerDigits > needed) {
        pow10 = uint64pow10[integerDigits - needed];
        integer *= uint32_t(pow10);
        rest -= integer * uint32_t(pow10);
    } else {
        rest = 0;
    }
    pos = len(buf);
    {
        uint32_t v{0};
        for (v = integer; v > 0;) {
            uint32_t v1{0};

            v1 = v / 10;
            v -= 10 * v1;
            pos--;
            buf[pos] = byte(v + '0');
            v = v1;
        }
    }
    {
        int i{0};
        for (i = pos; i < len(buf); i++) {
            d->d[i - pos] = buf[i];
        }
    }
    nd = len(buf) - pos;
    d->nd = nd;
    d->dp = integerDigits + exp10;
    needed -= nd;
    if (needed > 0) {
        if (rest != 0 || pow10 != 1) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 1385147) STRING
                         "strconv: internal error, rest != 0 but needed > 0"}));
        }
        while (needed > 0) {
            uint64_t digit{0};

            fraction *= 10;
            ε *= 10;
            if (2 * ε > 1 << shift) {
                return false;
            }
            digit = fraction >> shift;
            d->d[nd] = byte(digit + '0');
            fraction -= digit << shift;
            nd++;
            needed--;
        }
        d->nd = nd;
    }
    ok = adjustLastDigitFixed(
            d, uint64_t(rest) << shift | fraction, pow10, shift, ε);
    if (!ok) {
        return false;
    }
    {
        int i{0};
        for (i = d->nd - 1; i >= 0; i--) {
            if (d->d[i] != '0') {
                d->nd = i + 1;
                break;
            }
        }
    }
    return true;
}
bool adjustLastDigitFixed(decimalSlice *d,
                          uint64_t num,
                          uint64_t den,
                          unsigned int shift,
                          uint64_t ε)
{
    if (num > den << shift) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 1387104) STRING
                     "strconv: num > den<<shift in adjustLastDigitFixed"}));
    }
    if (2 * ε > den << shift) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 1387192) STRING
                     "strconv: ε > (den<<shift)/2"}));
    }
    if (2 * (num + ε) < den << shift) {
        return true;
    }
    if (2 * (num - ε) > den << shift) {
        i = d->nd - 1;
        for (; i >= 0; i--) {
            if (d->d[i] == '9') {
                d->nd--;
            } else {
                break;
            }
        }
        if (i < 0) {
            d->d[0] = '1';
            d->nd = 1;
            d->dp++;
        } else {
            d->d[i]++;
        }
        return true;
    }
    return false;
}
bool extFloat::ShortestDecimal(decimalSlice *d,
                               extFloat *lower,
                               extFloat *upper)
{
    uint64_t allowance{0};
    int digit{0};
    int exp10{0};
    uint64_t fraction{0};
    uint32_t integer{0};
    int integerDigits{0};
    uint64_t multiplier{0};
    unsigned int shift{0};
    uint64_t targetDiff{0};

    if (this->mant == 0) {
        d->nd = 0;
        d->dp = 0;
        d->neg = this->neg;
        return true;
    }
    if (this->exp == 0 && *lower == *this && *lower == *upper) {
        n = len(buf) - 1;
        {
            uint64_t v{0};
            for (v = this->mant; v > 0;) {
                uint64_t v1{0};

                v1 = v / 10;
                v -= 10 * v1;
                buf[n] = byte(v + '0');
                n--;
                v = v1;
            }
        }
        nd = len(buf) - n - 1;
        {
            int i{0};
            for (i = 0; i < nd; i++) {
                d->d[i] = buf[n + 1 + i];
            }
        }
        std::tie(d->nd, d->dp) = std::tuple<int, int>(nd, nd);
        while (d->nd > 0 && d->d[d->nd - 1] == '0') {
            d->nd--;
        }
        if (d->nd == 0) {
            d->dp = 0;
        }
        d->neg = this->neg;
        return true;
    }
    upper->Normalize();
    if (this->exp > upper->exp) {
        this->mant <<= unsigned int(this->exp - upper->exp);
        this->exp = upper->exp;
    }
    if (lower->exp > upper->exp) {
        lower->mant <<= unsigned int(lower->exp - upper->exp);
        lower->exp = upper->exp;
    }
    exp10 = frexp10Many(lower, this, upper);
    upper->mant++;
    lower->mant--;
    shift = unsigned int(-upper->exp);
    integer = uint32_t(upper->mant >> shift);
    fraction = upper->mant - (uint64_t(integer) << shift);
    allowance = upper->mant - lower->mant;
    targetDiff = upper->mant - this->mant;
    {
        int i{0};
        uint64_t pow{0};
        for (std::tie(i, pow) = std::tuple<int, uint64_t>(0, uint64_t(1));
             i < 20;
             i++) {
            if (pow > uint64_t(integer)) {
                integerDigits = i;
                break;
            }
            pow *= 10;
        }
    }
    {
        int i{0};
        for (i = 0; i < integerDigits; i++) {
            uint32_t digit{0};
            uint64_t pow{0};

            pow = uint64pow10[integerDigits - i - 1];
            digit = integer / uint32_t(pow);
            d->d[i] = byte(digit + '0');
            integer -= digit * uint32_t(pow);
            {
                uint64_t currentDiff{0};

                currentDiff = uint64_t(integer) << shift + fraction;

                if (currentDiff < allowance) {
                    d->nd = i + 1;
                    d->dp = integerDigits + exp10;
                    d->neg = this->neg;
                    return adjustLastDigit(d,
                                           currentDiff,
                                           targetDiff,
                                           allowance,
                                           pow << shift,
                                           2);
                }
            }
        }
    }
    d->nd = integerDigits;
    d->dp = d->nd + exp10;
    d->neg = this->neg;
    multiplier = uint64_t(1);
    while (true) {
        fraction *= 10;
        multiplier *= 10;
        digit = int(fraction >> shift);
        d->d[d->nd] = byte(digit + '0');
        d->nd++;
        fraction -= uint64_t(digit) << shift;
        if (fraction < allowance * multiplier) {
            return adjustLastDigit(d,
                                   fraction,
                                   targetDiff * multiplier,
                                   allowance * multiplier,
                                   1 << shift,
                                   multiplier * 2);
        }
    }
}
bool adjustLastDigit(decimalSlice *d,
                     uint64_t currentDiff,
                     uint64_t targetDiff,
                     uint64_t maxDiff,
                     uint64_t ulpDecimal,
                     uint64_t ulpBinary)
{
    if (ulpDecimal < 2 * ulpBinary) {
        return false;
    }
    while (currentDiff + ulpDecimal / 2 + ulpBinary < targetDiff) {
        d->d[d->nd - 1]--;
        currentDiff += ulpDecimal;
    }
    if (currentDiff + ulpDecimal <= targetDiff + ulpDecimal / 2 + ulpBinary) {
        return false;
    }
    if (currentDiff < ulpBinary || currentDiff > maxDiff - ulpBinary) {
        return false;
    }
    if (d->nd == 1 && d->d[0] == '0') {
        d->nd = 0;
        d->dp = 0;
    }
    return true;
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
std::string FormatFloat(double f, uint8_t fmt, int prec, int bitSize)
{
    return std::string(
            genericFtoa(make(moku::slice<byte>, 0, max(prec + 4, 24)),
                        f,
                        fmt,
                        prec,
                        bitSize));
}
moku::slice<uint8_t> AppendFloat(
        moku::slice<uint8_t> dst, double f, uint8_t fmt, int prec, int bitSize)
{
    return genericFtoa(dst, f, fmt, prec, bitSize);
}
moku::slice<uint8_t> genericFtoa(moku::slice<uint8_t> dst,
                                 double val,
                                 uint8_t fmt,
                                 int prec,
                                 int bitSize)
{
    uint64_t bits{0};
    decimalSlice digs{};
    int exp{0};
    floatInfo *flt{std::nullptr};
    uint64_t mant{0};
    bool neg{false};
    bool ok{false};
    bool shortest{false};

    if ((bitSize == 32)) {
    _ident_0_:
        bits = uint64_t(math::Float32bits(float(val)));
        flt = &float32info;
    } else if ((bitSize == 64)) {
    _ident_1_:
        bits = math::Float64bits(val);
        flt = &float64info;
    } else {
    _ident_2_:
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 1418947) STRING
                     "strconv: illegal AppendFloat/FormatFloat bitSize"}));
    }
    neg = bits >> (flt->expbits + flt->mantbits) != 0;
    exp = int(bits >> flt->mantbits) & (1 << flt->expbits - 1);
    mant = bits & (uint64_t(1) << flt->mantbits - 1);
    if ((exp == 1 << flt->expbits - 1)) {
    _ident_3_:
        if ((mant != 0)) {
        _ident_6_:
            s = "NaN";
        } else if ((neg)) {
        _ident_7_:
            s = "-Inf";
        } else {
        _ident_8_:
            s = "+Inf";
        }
        return append(dst, s);
    } else if ((exp == 0)) {
    _ident_4_:
        exp++;
    } else {
    _ident_5_:
        mant |= uint64_t(1) << flt->mantbits;
    }
    exp += flt->bias;
    if (fmt == 'b') {
        return fmtB(dst, neg, mant, exp, flt);
    }
    if (!optimize) {
        return bigFtoa(dst, prec, fmt, neg, mant, exp, flt);
    }
    ok = false;
    shortest = prec < 0;
    if (shortest) {
        f = new (extFloat);
        std::tie(lower, upper) = f->AssignComputeBounds(mant, exp, neg, flt);
        digs.d = moku::slice_expr<std::vector<uint8_t>>(buf);
        ok = f->ShortestDecimal(&digs, &lower, &upper);
        if (!ok) {
            return bigFtoa(dst, prec, fmt, neg, mant, exp, flt);
        }
        if ((fmt == 'e') || (fmt == 'E')) {
        _ident_9_:
            prec = max(digs.nd - 1, 0);
        } else if ((fmt == 'f')) {
        _ident_10_:
            prec = max(digs.nd - digs.dp, 0);
        } else if ((fmt == 'g') || (fmt == 'G')) {
        _ident_11_:
            prec = digs.nd;
        }
    } else {
        if (fmt != 'f') {
            digits = prec;
            if ((fmt == 'e') || (fmt == 'E')) {
            _ident_12_:
                digits++;
            } else if ((fmt == 'g') || (fmt == 'G')) {
            _ident_13_:
                if (prec == 0) {
                    prec = 1;
                }
                digits = prec;
            }
            if (digits <= 15) {
                digs.d = moku::slice_expr<std::vector<uint8_t>>(buf);
                f = extFloat{mant, exp - int(flt->mantbits), neg};
                ok = f.FixedDecimal(&digs, digits);
            }
        }
    }
    if (!ok) {
        return bigFtoa(dst, prec, fmt, neg, mant, exp, flt);
    }
    return formatDigits(dst, shortest, neg, digs, prec, fmt);
}
moku::slice<uint8_t> bigFtoa(moku::slice<uint8_t> dst,
                             int prec,
                             uint8_t fmt,
                             bool neg,
                             uint64_t mant,
                             int exp,
                             floatInfo *flt)
{
    decimal *d{std::nullptr};
    decimalSlice digs{};
    bool shortest{false};

    d = new (decimal);
    d->Assign(mant);
    d->Shift(exp - int(flt->mantbits));
    shortest = prec < 0;
    if (shortest) {
        roundShortest(d, mant, exp, flt);
        digs = decimalSlice{
            d : moku::slice_expr<std::vector<uint8_t>>(d->d),
            nd : d->nd,
            dp : d->dp
        };
        if ((fmt == 'e') || (fmt == 'E')) {
        _ident_14_:
            prec = digs.nd - 1;
        } else if ((fmt == 'f')) {
        _ident_15_:
            prec = max(digs.nd - digs.dp, 0);
        } else if ((fmt == 'g') || (fmt == 'G')) {
        _ident_16_:
            prec = digs.nd;
        }
    } else {
        if ((fmt == 'e') || (fmt == 'E')) {
        _ident_17_:
            d->Round(prec + 1);
        } else if ((fmt == 'f')) {
        _ident_18_:
            d->Round(d->dp + prec);
        } else if ((fmt == 'g') || (fmt == 'G')) {
        _ident_19_:
            if (prec == 0) {
                prec = 1;
            }
            d->Round(prec);
        }
        digs = decimalSlice{
            d : moku::slice_expr<std::vector<uint8_t>>(d->d),
            nd : d->nd,
            dp : d->dp
        };
    }
    return formatDigits(dst, shortest, neg, digs, prec, fmt);
}
moku::slice<uint8_t> formatDigits(moku::slice<uint8_t> dst,
                                  bool shortest,
                                  bool neg,
                                  decimalSlice digs,
                                  int prec,
                                  uint8_t fmt)
{
    if ((fmt == 'e') || (fmt == 'E')) {
    _ident_20_:
        return fmtE(dst, neg, digs, prec, fmt);
    } else if ((fmt == 'f')) {
    _ident_21_:
        return fmtF(dst, neg, digs, prec);
    } else if ((fmt == 'g') || (fmt == 'G')) {
    _ident_22_:
        eprec = prec;
        if (eprec > digs.nd && digs.nd >= digs.dp) {
            eprec = digs.nd;
        }
        if (shortest) {
            eprec = 6;
        }
        exp = digs.dp - 1;
        if (exp < -4 || exp >= eprec) {
            if (prec > digs.nd) {
                prec = digs.nd;
            }
            return fmtE(dst, neg, digs, prec - 1, fmt + 'e' - 'g');
        }
        if (prec > digs.dp) {
            prec = digs.nd;
        }
        return fmtF(dst, neg, digs, max(prec - digs.dp, 0));
    }
    return append(dst, '%', fmt);
}
void roundShortest(decimal *d, uint64_t mant, int exp, floatInfo *flt)
{
    int explo{0};
    bool inclusive{false};
    decimal *lower{std::nullptr};
    uint64_t mantlo{0};
    int minexp{0};
    decimal *upper{std::nullptr};

    if (mant == 0) {
        d->nd = 0;
        return;
    }
    minexp = flt->bias + 1;
    if (exp > minexp &&
        332 * (d->dp - d->nd) >= 100 * (exp - int(flt->mantbits))) {
        return;
    }
    upper = new (decimal);
    upper->Assign(mant * 2 + 1);
    upper->Shift(exp - int(flt->mantbits) - 1);
    if (mant > 1 << flt->mantbits || exp == minexp) {
        mantlo = mant - 1;
        explo = exp;
    } else {
        mantlo = mant * 2 - 1;
        explo = exp - 1;
    }
    lower = new (decimal);
    lower->Assign(mantlo * 2 + 1);
    lower->Shift(explo - int(flt->mantbits) - 1);
    inclusive = mant % 2 == 0;
    {
        int i{0};
        for (i = 0; i < d->nd; i++) {
            uint8_t l{0};
            uint8_t m{0};
            bool okdown{false};
            bool okup{false};
            uint8_t u{0};

            l = byte('0');
            if (i < lower->nd) {
                l = lower->d[i];
            }
            m = d->d[i];
            u = byte('0');
            if (i < upper->nd) {
                u = upper->d[i];
            }
            okdown = l != m || inclusive && i + 1 == lower->nd;
            okup = m != u && (inclusive || m + 1 < u || i + 1 < upper->nd);
            if ((okdown && okup)) {
            _ident_23_:
                d->Round(i + 1);
                return;
            } else if ((okdown)) {
            _ident_24_:
                d->RoundDown(i + 1);
                return;
            } else if ((okup)) {
            _ident_25_:
                d->RoundUp(i + 1);
                return;
            }
        }
    }
}
moku::slice<uint8_t> fmtE(moku::slice<uint8_t> dst,
                          bool neg,
                          decimalSlice d,
                          int prec,
                          uint8_t fmt)
{
    uint8_t ch{0};
    int exp{0};

    if (neg) {
        dst = append(dst, '-');
    }
    ch = byte('0');
    if (d.nd != 0) {
        ch = d.d[0];
    }
    dst = append(dst, ch);
    if (prec > 0) {
        dst = append(dst, '.');
        i = 1;
        m = min(d.nd, prec + 1);
        if (i < m) {
            dst = append(dst,
                         moku::slice_expr<moku::slice<uint8_t>>(d.d, i, m));
            i = m;
        }
        for (; i <= prec; i++) {
            dst = append(dst, '0');
        }
    }
    dst = append(dst, fmt);
    exp = d.dp - 1;
    if (d.nd == 0) {
        exp = 0;
    }
    if (exp < 0) {
        ch = '-';
        exp = -exp;
    } else {
        ch = '+';
    }
    dst = append(dst, ch);
    if ((exp < 10)) {
    _ident_26_:
        dst = append(dst, '0', byte(exp) + '0');
    } else if ((exp < 100)) {
    _ident_27_:
        dst = append(dst, byte(exp / 10) + '0', byte(exp % 10) + '0');
    } else {
    _ident_28_:
        dst = append(dst,
                     byte(exp / 100) + '0',
                     byte(exp / 10) % 10 + '0',
                     byte(exp % 10) + '0');
    }
    return dst;
}
moku::slice<uint8_t> fmtF(moku::slice<uint8_t> dst,
                          bool neg,
                          decimalSlice d,
                          int prec)
{
    if (neg) {
        dst = append(dst, '-');
    }
    if (d.dp > 0) {
        m = min(d.nd, d.dp);
        dst = append(dst, moku::slice_expr<moku::slice<uint8_t>>(d.d, m));
        for (; m < d.dp; m++) {
            dst = append(dst, '0');
        }
    } else {
        dst = append(dst, '0');
    }
    if (prec > 0) {
        dst = append(dst, '.');
        {
            int i{0};
            for (i = 0; i < prec; i++) {
                uint8_t ch{0};

                ch = byte('0');
                {
                    int j{0};

                    j = d.dp + i;

                    if (0 <= j && j < d.nd) {
                        ch = d.d[j];
                    }
                }
                dst = append(dst, ch);
            }
        }
    }
    return dst;
}
moku::slice<uint8_t> fmtB(moku::slice<uint8_t> dst,
                          bool neg,
                          uint64_t mant,
                          int exp,
                          floatInfo *flt)
{
    if (neg) {
        dst = append(dst, '-');
    }
    std::tie(dst, _) = formatBits(dst, mant, 10, false, true);
    dst = append(dst, 'p');
    exp -= int(flt->mantbits);
    if (exp >= 0) {
        dst = append(dst, '+');
    }
    std::tie(dst, _) = formatBits(dst, uint64_t(exp), 10, exp < 0, true);
    return dst;
}
int min(int a, int b)
{
    if (a < b) {
        return a;
    }
    return b;
}
int max(int a, int b)
{
    if (a > b) {
        return a;
    }
    return b;
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
std::string FormatUint(uint64_t i, int base)
{
    std::string s{""};

    if (fastSmalls && i < nSmalls && base == 10) {
        return small(int(i));
    }
    std::tie(_, s) = formatBits(nil, i, base, false, false);
    return s;
}
std::string FormatInt(int64_t i, int base)
{
    std::string s{""};

    if (fastSmalls && 0 <= i && i < nSmalls && base == 10) {
        return small(int(i));
    }
    std::tie(_, s) = formatBits(nil, uint64_t(i), base, i < 0, false);
    return s;
}
std::string Itoa(int i)
{
    return FormatInt(int64_t(i), 10);
}
moku::slice<uint8_t> AppendInt(moku::slice<uint8_t> dst, int64_t i, int base)
{
    if (fastSmalls && 0 <= i && i < nSmalls && base == 10) {
        return append(dst, small(int(i)));
    }
    std::tie(dst, _) = formatBits(dst, uint64_t(i), base, i < 0, true);
    return dst;
}
moku::slice<uint8_t> AppendUint(moku::slice<uint8_t> dst, uint64_t i, int base)
{
    if (fastSmalls && i < nSmalls && base == 10) {
        return append(dst, small(int(i)));
    }
    std::tie(dst, _) = formatBits(dst, i, base, false, true);
    return dst;
}
std::string small(int i)
{
    int off{0};

    off = 0;
    if (i < 10) {
        off = 1;
    }
    return moku::slice_expr<std::string>(smallsString, i * 2 + off, i * 2 + 2);
}
std::tuple<moku::slice<uint8_t>, std::string> formatBits(
        moku::slice<uint8_t> dst, uint64_t u, int base, bool neg, bool append_)
{
    std::vector<uint8_t> a{};
    moku::slice<uint8_t> d{};
    int i{0};
    std::string s{""};

    if (base < 2 || base > len(digits)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 1430794) STRING
                     "strconv: illegal AppendInt/FormatInt base"}));
    }
    i = len(a);
    if (neg) {
        u = -u;
    }
    if (base == 10) {
        if (host32bit) {
            while (u >= 1e9) {
                uint64_t q{0};
                unsigned int us{0};

                q = u / 1e9;
                us = unsigned int(u - q * 1e9);
                {
                    int j{0};
                    for (j = 4; j > 0; j--) {
                        unsigned int is{0};

                        is = us % 100 * 2;
                        us *= 100;
                        i -= 2;
                        a[i + 1] = smallsString[is + 1];
                        a[i + 0] = smallsString[is + 0];
                    }
                }
                i--;
                a[i] = smallsString[us * 2 + 1];
                u = q;
            }
        }
        us = unsigned int(u);
        while (us >= 100) {
            unsigned int is{0};

            is = us % 100 * 2;
            us *= 100;
            i -= 2;
            a[i + 1] = smallsString[is + 1];
            a[i + 0] = smallsString[is + 0];
        }
        is = us * 2;
        i--;
        a[i] = smallsString[is + 1];
        if (us >= 10) {
            i--;
            a[i] = smallsString[is];
        }
    } else {
        {
            unsigned int s{0};

            s = shifts[base];

            if (s > 0) {
                b = uint64_t(base);
                m = unsigned int(base) - 1;
                while (u >= b) {
                    i--;
                    a[i] = digits[unsigned int(u) & m];
                    u >>= s;
                }
                i--;
                a[i] = digits[unsigned int(u)];
            } else {
                b = uint64_t(base);
                while (u >= b) {
                    uint64_t q{0};

                    i--;
                    q = u / b;
                    a[i] = digits[unsigned int(u - q * b)];
                    u = q;
                }
                i--;
                a[i] = digits[unsigned int(u)];
            }
        }
    }
    if (neg) {
        i--;
        a[i] = '-';
    }
    if (append_) {
        d = append(dst, moku::slice_expr<std::vector<uint8_t>>(a, i));
        return;
    }
    s = std::string(moku::slice_expr<std::vector<uint8_t>>(a, i));
    return;
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
std::string quoteWith(std::string s,
                      uint8_t quote,
                      bool ASCIIonly,
                      bool graphicOnly)
{
    return std::string(
            appendQuotedWith(make(moku::slice<byte>, 0, 3 * len(s) / 2),
                             s,
                             quote,
                             ASCIIonly,
                             graphicOnly));
}
std::string quoteRuneWith(int32_t r,
                          uint8_t quote,
                          bool ASCIIonly,
                          bool graphicOnly)
{
    return std::string(
            appendQuotedRuneWith(nil, r, quote, ASCIIonly, graphicOnly));
}
moku::slice<uint8_t> appendQuotedWith(moku::slice<uint8_t> buf,
                                      std::string s,
                                      uint8_t quote,
                                      bool ASCIIonly,
                                      bool graphicOnly)
{
    buf = append(buf, quote);
    {
        int width{0};
        for (width = 0; len(s) > 0;
             s = moku::slice_expr<std::string>(s, width)) {
            int32_t r{0};

            r = rune(s[0]);
            width = 1;
            if (r >= utf8::RuneSelf) {
                std::tie(r, width) = utf8::DecodeRuneInString(s);
            }
            if (width == 1 && r == utf8::RuneError) {
                buf = append(buf, `\x`);
                buf = append(buf, lowerhex[s[0] >> 4]);
                buf = append(buf, lowerhex[s[0] & 0xF]);
                continue;
            }
            buf = appendEscapedRune(buf, r, quote, ASCIIonly, graphicOnly);
        }
    }
    buf = append(buf, quote);
    return buf;
}
moku::slice<uint8_t> appendQuotedRuneWith(moku::slice<uint8_t> buf,
                                          int32_t r,
                                          uint8_t quote,
                                          bool ASCIIonly,
                                          bool graphicOnly)
{
    buf = append(buf, quote);
    if (!utf8::ValidRune(r)) {
        r = utf8::RuneError;
    }
    buf = appendEscapedRune(buf, r, quote, ASCIIonly, graphicOnly);
    buf = append(buf, quote);
    return buf;
}
moku::slice<uint8_t> appendEscapedRune(moku::slice<uint8_t> buf,
                                       int32_t r,
                                       uint8_t quote,
                                       bool ASCIIonly,
                                       bool graphicOnly)
{
    std::vector<uint8_t> runeTmp{};

    if (r == rune(quote) || r == '\\') {
        buf = append(buf, '\\');
        buf = append(buf, byte(r));
        return buf;
    }
    if (ASCIIonly) {
        if (r < utf8::RuneSelf && IsPrint(r)) {
            buf = append(buf, byte(r));
            return buf;
        }
    } else {
        if (IsPrint(r) || graphicOnly && isInGraphicList(r)) {
            n = utf8::EncodeRune(
                    moku::slice_expr<std::vector<uint8_t>>(runeTmp), r);
            buf = append(buf,
                         moku::slice_expr<std::vector<uint8_t>>(runeTmp, n));
            return buf;
        }
    }
    if ((r == '\a')) {
    _ident_0_:
        buf = append(buf, `\a`);
    } else if ((r == '\b')) {
    _ident_1_:
        buf = append(buf, `\b`);
    } else if ((r == '\f')) {
    _ident_2_:
        buf = append(buf, `\f`);
    } else if ((r == '\n')) {
    _ident_3_:
        buf = append(buf, `\n`);
    } else if ((r == '\r')) {
    _ident_4_:
        buf = append(buf, `\r`);
    } else if ((r == '\t')) {
    _ident_5_:
        buf = append(buf, `\t`);
    } else if ((r == '\v')) {
    _ident_6_:
        buf = append(buf, `\v`);
    } else {
    _ident_7_:
        if ((r < ' ')) {
        _ident_8_:
            buf = append(buf, `\x`);
            buf = append(buf, lowerhex[byte(r) >> 4]);
            buf = append(buf, lowerhex[byte(r) & 0xF]);
        } else if ((r > utf8::MaxRune)) {
        _ident_9_:
            r = 0xFFFD;
            goto _ident_10_;
        } else if ((r < 0x10000)) {
        _ident_10_:
            buf = append(buf, `\u`);
            {
                int s{0};
                for (s = 12; s >= 0; s -= 4) {
                    buf = append(buf, lowerhex[r >> unsigned int(s) & 0xF]);
                }
            }
        } else {
        _ident_11_:
            buf = append(buf, `\U`);
            {
                int s{0};
                for (s = 28; s >= 0; s -= 4) {
                    buf = append(buf, lowerhex[r >> unsigned int(s) & 0xF]);
                }
            }
        }
    }
    return buf;
}
std::string Quote(std::string s)
{
    return quoteWith(s, '"', false, false);
}
moku::slice<uint8_t> AppendQuote(moku::slice<uint8_t> dst, std::string s)
{
    return appendQuotedWith(dst, s, '"', false, false);
}
std::string QuoteToASCII(std::string s)
{
    return quoteWith(s, '"', true, false);
}
moku::slice<uint8_t> AppendQuoteToASCII(moku::slice<uint8_t> dst, std::string s)
{
    return appendQuotedWith(dst, s, '"', true, false);
}
std::string QuoteToGraphic(std::string s)
{
    return quoteWith(s, '"', false, true);
}
moku::slice<uint8_t> AppendQuoteToGraphic(moku::slice<uint8_t> dst,
                                          std::string s)
{
    return appendQuotedWith(dst, s, '"', false, true);
}
std::string QuoteRune(int32_t r)
{
    return quoteRuneWith(r, '\'', false, false);
}
moku::slice<uint8_t> AppendQuoteRune(moku::slice<uint8_t> dst, int32_t r)
{
    return appendQuotedRuneWith(dst, r, '\'', false, false);
}
std::string QuoteRuneToASCII(int32_t r)
{
    return quoteRuneWith(r, '\'', true, false);
}
moku::slice<uint8_t> AppendQuoteRuneToASCII(moku::slice<uint8_t> dst, int32_t r)
{
    return appendQuotedRuneWith(dst, r, '\'', true, false);
}
std::string QuoteRuneToGraphic(int32_t r)
{
    return quoteRuneWith(r, '\'', false, true);
}
moku::slice<uint8_t> AppendQuoteRuneToGraphic(moku::slice<uint8_t> dst,
                                              int32_t r)
{
    return appendQuotedRuneWith(dst, r, '\'', false, true);
}
bool CanBackquote(std::string s)
{
    while (len(s) > 0) {
        int32_t r{0};
        int wid{0};

        std::tie(r, wid) = utf8::DecodeRuneInString(s);
        s = moku::slice_expr<std::string>(s, wid);
        if (wid > 1) {
            if (r == '\ufeff') {
                return false;
            }
            continue;
        }
        if (r == utf8::RuneError) {
            return false;
        }
        if ((r < ' ' && r != '\t') || r == '`' || r == '\u007F') {
            return false;
        }
    }
    return true;
}
std::tuple<int32_t, bool> unhex(uint8_t b)
{
    int32_t c{0};
    bool ok{false};
    int32_t v{0};

    c = rune(b);
    if (('0' <= c && c <= '9')) {
    _ident_12_:
        return {c - '0', true};
    } else if (('a' <= c && c <= 'f')) {
    _ident_13_:
        return {c - 'a' + 10, true};
    } else if (('A' <= c && c <= 'F')) {
    _ident_14_:
        return {c - 'A' + 10, true};
    }
    return;
}
std::tuple<int32_t, bool, std::string, moku::error> UnquoteChar(std::string s,
                                                                uint8_t quote)
{
    uint8_t c{0};
    moku::error err{};
    bool multibyte{false};
    std::string tail{""};
    int32_t value{0};

    {
        uint8_t c{0};
        c = s[0];
        if ((c == quote && (quote == '\'' || quote == '"'))) {
        _ident_15_:
            err = ErrSyntax;
            return;
        } else if ((c >= utf8::RuneSelf)) {
        _ident_16_:
            std::tie(r, size) = utf8::DecodeRuneInString(s);
            return {r, true, moku::slice_expr<std::string>(s, size), nil};
        } else if ((c != '\\')) {
        _ident_17_:
            return {rune(s[0]),
                    false,
                    moku::slice_expr<std::string>(s, 1),
                    nil};
        }
    }
    if (len(s) <= 1) {
        err = ErrSyntax;
        return;
    }
    c = s[1];
    s = moku::slice_expr<std::string>(s, 2);
    if ((c == 'a')) {
    _ident_18_:
        value = '\a';
    } else if ((c == 'b')) {
    _ident_19_:
        value = '\b';
    } else if ((c == 'f')) {
    _ident_20_:
        value = '\f';
    } else if ((c == 'n')) {
    _ident_21_:
        value = '\n';
    } else if ((c == 'r')) {
    _ident_22_:
        value = '\r';
    } else if ((c == 't')) {
    _ident_23_:
        value = '\t';
    } else if ((c == 'v')) {
    _ident_24_:
        value = '\v';
    } else if ((c == 'x') || (c == 'u') || (c == 'U')) {
    _ident_25_:
        n = 0;
        if ((c == 'x')) {
        _ident_30_:
            n = 2;
        } else if ((c == 'u')) {
        _ident_31_:
            n = 4;
        } else if ((c == 'U')) {
        _ident_32_:
            n = 8;
        }
        if (len(s) < n) {
            err = ErrSyntax;
            return;
        }
        {
            int j{0};
            for (j = 0; j < n; j++) {
                bool ok{false};
                int32_t x{0};

                std::tie(x, ok) = unhex(s[j]);
                if (!ok) {
                    err = ErrSyntax;
                    return;
                }
                v = v << 4 | x;
            }
        }
        s = moku::slice_expr<std::string>(s, n);
        if (c == 'x') {
            value = v;
            break;
        }
        if (v > utf8::MaxRune) {
            err = ErrSyntax;
            return;
        }
        value = v;
        multibyte = true;
    } else if ((c == '0') || (c == '1') || (c == '2') || (c == '3') ||
               (c == '4') || (c == '5') || (c == '6') || (c == '7')) {
    _ident_26_:
        v = rune(c) - '0';
        if (len(s) < 2) {
            err = ErrSyntax;
            return;
        }
        {
            int j{0};
            for (j = 0; j < 2; j++) {
                int32_t x{0};

                x = rune(s[j]) - '0';
                if (x < 0 || x > 7) {
                    err = ErrSyntax;
                    return;
                }
                v = (v << 3) | x;
            }
        }
        s = moku::slice_expr<std::string>(s, 2);
        if (v > 255) {
            err = ErrSyntax;
            return;
        }
        value = v;
    } else if ((c == '\\')) {
    _ident_27_:
        value = '\\';
    } else if ((c == '\'') || (c == '"')) {
    _ident_28_:
        if (c != quote) {
            err = ErrSyntax;
            return;
        }
        value = rune(c);
    } else {
    _ident_29_:
        err = ErrSyntax;
        return;
    }
    tail = s;
    return;
}
std::tuple<std::string, moku::error> Unquote(std::string s)
{
    moku::slice<uint8_t> buf{};
    int n{0};
    uint8_t quote{0};
    std::vector<uint8_t> runeTmp{};

    n = len(s);
    if (n < 2) {
        return {"", ErrSyntax};
    }
    quote = s[0];
    if (quote != s[n - 1]) {
        return {"", ErrSyntax};
    }
    s = moku::slice_expr<std::string>(s, 1, n - 1);
    if (quote == '`') {
        if (contains(s, '`')) {
            return {"", ErrSyntax};
        }
        if (contains(s, '\r')) {
            buf = make(moku::slice<byte>, 0, len(s) - 1);
            {
                int i{0};
                for (i = 0; i < len(s); i++) {
                    if (s[i] != '\r') {
                        buf = append(buf, s[i]);
                    }
                }
            }
            return {std::string(buf), nil};
        }
        return {s, nil};
    }
    if (quote != '"' && quote != '\'') {
        return {"", ErrSyntax};
    }
    if (contains(s, '\n')) {
        return {"", ErrSyntax};
    }
    if (!contains(s, '\\') && !contains(s, quote)) {
        if ((quote == '"')) {
        _ident_33_:
            return {s, nil};
        } else if ((quote == '\'')) {
        _ident_34_:
            std::tie(r, size) = utf8::DecodeRuneInString(s);
            if (size == len(s) && (r != utf8::RuneError || size != 1)) {
                return {s, nil};
            }
        }
    }
    buf = make(moku::slice<byte>, 0, 3 * len(s) / 2);
    while (len(s) > 0) {
        int32_t c{0};
        moku::error err{};
        bool multibyte{false};
        std::string ss{""};

        std::tie(c, multibyte, ss, err) = UnquoteChar(s, quote);
        if (!moku::is_nil(err)) {
            return {"", err};
        }
        s = ss;
        if (c < utf8::RuneSelf || !multibyte) {
            buf = append(buf, byte(c));
        } else {
            n = utf8::EncodeRune(
                    moku::slice_expr<std::vector<uint8_t>>(runeTmp), c);
            buf = append(buf,
                         moku::slice_expr<std::vector<uint8_t>>(runeTmp, n));
        }
        if (quote == '\'' && len(s) != 0) {
            return {"", ErrSyntax};
        }
    }
    return {std::string(buf), nil};
}
bool contains(std::string s, uint8_t c)
{
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            if (s[i] == c) {
                return true;
            }
        }
    }
    return false;
}
int bsearch16(moku::slice<uint16_t> a, uint16_t x)
{
    int i{0};
    int j{0};

    std::tie(i, j) = std::tuple<int, int>(0, len(a));
    while (i < j) {
        int h{0};

        h = i + (j - i) / 2;
        if (a[h] < x) {
            i = h + 1;
        } else {
            j = h;
        }
    }
    return i;
}
int bsearch32(moku::slice<uint32_t> a, uint32_t x)
{
    int i{0};
    int j{0};

    std::tie(i, j) = std::tuple<int, int>(0, len(a));
    while (i < j) {
        int h{0};

        h = i + (j - i) / 2;
        if (a[h] < x) {
            i = h + 1;
        } else {
            j = h;
        }
    }
    return i;
}
bool IsPrint(int32_t r)
{
    int i{0};
    moku::slice<uint16_t> isNotPrint{};
    moku::slice<uint32_t> isPrint{};
    int j{0};
    uint32_t rr{0};

    if (r <= 0xFF) {
        if (0x20 <= r && r <= 0x7E) {
            return true;
        }
        if (0xA1 <= r && r <= 0xFF) {
            return r != 0xAD;
        }
        return false;
    }
    if (0 <= r && r < 1 << 16) {
        std::tie(rr, isPrint, isNotPrint) = std::
                tuple<uint16_t, moku::slice<uint16_t>, moku::slice<uint16_t>>(
                        uint16_t(r), isPrint16, isNotPrint16);
        i = bsearch16(isPrint, rr);
        if (i >= len(isPrint) || rr < isPrint[i & ~(1)] ||
            isPrint[i | 1] < rr) {
            return false;
        }
        j = bsearch16(isNotPrint, rr);
        return j >= len(isNotPrint) || isNotPrint[j] != rr;
    }
    std::tie(rr, isPrint, isNotPrint) =
            std::tuple<uint32_t, moku::slice<uint32_t>, moku::slice<uint16_t>>(
                    uint32_t(r), isPrint32, isNotPrint32);
    i = bsearch32(isPrint, rr);
    if (i >= len(isPrint) || rr < isPrint[i & ~(1)] || isPrint[i | 1] < rr) {
        return false;
    }
    if (r >= 0x20000) {
        return true;
    }
    r -= 0x10000;
    j = bsearch16(isNotPrint, uint16_t(r));
    return j >= len(isNotPrint) || isNotPrint[j] != uint16_t(r);
}
bool IsGraphic(int32_t r)
{
    if (IsPrint(r)) {
        return true;
    }
    return isInGraphicList(r);
}
bool isInGraphicList(int32_t r)
{
    int i{0};
    uint16_t rr{0};

    if (r > 0xFFFF) {
        return false;
    }
    rr = uint16_t(r);
    i = bsearch16(isGraphic, rr);
    return i < len(isGraphic) && rr == isGraphic[i];
}
int main()
{
    optimize = true;
    powtab = moku::slice<int>{1, 3, 6, 9, 13, 16, 19, 23, 26};
    float64pow10 =
            moku::slice<double>{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
                                1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
                                1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
    float32pow10 = moku::slice<float>{
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10};
    ErrRange = errors::New("value out of range");
    ErrSyntax = errors::New("invalid syntax");
    leftcheats = moku::slice<leftCheat>{
            {0, ""},
            {1, "5"},
            {1, "25"},
            {1, "125"},
            {2, "625"},
            {2, "3125"},
            {2, "15625"},
            {3, "78125"},
            {3, "390625"},
            {3, "1953125"},
            {4, "9765625"},
            {4, "48828125"},
            {4, "244140625"},
            {4, "1220703125"},
            {5, "6103515625"},
            {5, "30517578125"},
            {5, "152587890625"},
            {6, "762939453125"},
            {6, "3814697265625"},
            {6, "19073486328125"},
            {7, "95367431640625"},
            {7, "476837158203125"},
            {7, "2384185791015625"},
            {7, "11920928955078125"},
            {8, "59604644775390625"},
            {8, "298023223876953125"},
            {8, "1490116119384765625"},
            {9, "7450580596923828125"},
            {9, "37252902984619140625"},
            {9, "186264514923095703125"},
            {10, "931322574615478515625"},
            {10, "4656612873077392578125"},
            {10, "23283064365386962890625"},
            {10, "116415321826934814453125"},
            {11, "582076609134674072265625"},
            {11, "2910383045673370361328125"},
            {11, "14551915228366851806640625"},
            {12, "72759576141834259033203125"},
            {12, "363797880709171295166015625"},
            {12, "1818989403545856475830078125"},
            {13, "9094947017729282379150390625"},
            {13, "45474735088646411895751953125"},
            {13, "227373675443232059478759765625"},
            {13, "1136868377216160297393798828125"},
            {14, "5684341886080801486968994140625"},
            {14, "28421709430404007434844970703125"},
            {14, "142108547152020037174224853515625"},
            {15, "710542735760100185871124267578125"},
            {15, "3552713678800500929355621337890625"},
            {15, "17763568394002504646778106689453125"},
            {16, "88817841970012523233890533447265625"},
            {16, "444089209850062616169452667236328125"},
            {16, "2220446049250313080847263336181640625"},
            {16, "11102230246251565404236316680908203125"},
            {17, "55511151231257827021181583404541015625"},
            {17, "277555756156289135105907917022705078125"},
            {17, "1387778780781445675529539585113525390625"},
            {18, "6938893903907228377647697925567626953125"},
            {18, "34694469519536141888238489627838134765625"},
            {18, "173472347597680709441192448139190673828125"},
            {19, "867361737988403547205962240695953369140625"}};
    smallPowersOfTen = std::vector<extFloat>{{1 << 63, -63, false},
                                             {0xa << 60, -60, false},
                                             {0x64 << 57, -57, false},
                                             {0x3e8 << 54, -54, false},
                                             {0x2710 << 50, -50, false},
                                             {0x186a0 << 47, -47, false},
                                             {0xf4240 << 44, -44, false},
                                             {0x989680 << 40, -40, false}};
    powersOfTen = std::vector<extFloat>{{0xfa8fd5a0081c0288, -1220, false},
                                        {0xbaaee17fa23ebf76, -1193, false},
                                        {0x8b16fb203055ac76, -1166, false},
                                        {0xcf42894a5dce35ea, -1140, false},
                                        {0x9a6bb0aa55653b2d, -1113, false},
                                        {0xe61acf033d1a45df, -1087, false},
                                        {0xab70fe17c79ac6ca, -1060, false},
                                        {0xff77b1fcbebcdc4f, -1034, false},
                                        {0xbe5691ef416bd60c, -1007, false},
                                        {0x8dd01fad907ffc3c, -980, false},
                                        {0xd3515c2831559a83, -954, false},
                                        {0x9d71ac8fada6c9b5, -927, false},
                                        {0xea9c227723ee8bcb, -901, false},
                                        {0xaecc49914078536d, -874, false},
                                        {0x823c12795db6ce57, -847, false},
                                        {0xc21094364dfb5637, -821, false},
                                        {0x9096ea6f3848984f, -794, false},
                                        {0xd77485cb25823ac7, -768, false},
                                        {0xa086cfcd97bf97f4, -741, false},
                                        {0xef340a98172aace5, -715, false},
                                        {0xb23867fb2a35b28e, -688, false},
                                        {0x84c8d4dfd2c63f3b, -661, false},
                                        {0xc5dd44271ad3cdba, -635, false},
                                        {0x936b9fcebb25c996, -608, false},
                                        {0xdbac6c247d62a584, -582, false},
                                        {0xa3ab66580d5fdaf6, -555, false},
                                        {0xf3e2f893dec3f126, -529, false},
                                        {0xb5b5ada8aaff80b8, -502, false},
                                        {0x87625f056c7c4a8b, -475, false},
                                        {0xc9bcff6034c13053, -449, false},
                                        {0x964e858c91ba2655, -422, false},
                                        {0xdff9772470297ebd, -396, false},
                                        {0xa6dfbd9fb8e5b88f, -369, false},
                                        {0xf8a95fcf88747d94, -343, false},
                                        {0xb94470938fa89bcf, -316, false},
                                        {0x8a08f0f8bf0f156b, -289, false},
                                        {0xcdb02555653131b6, -263, false},
                                        {0x993fe2c6d07b7fac, -236, false},
                                        {0xe45c10c42a2b3b06, -210, false},
                                        {0xaa242499697392d3, -183, false},
                                        {0xfd87b5f28300ca0e, -157, false},
                                        {0xbce5086492111aeb, -130, false},
                                        {0x8cbccc096f5088cc, -103, false},
                                        {0xd1b71758e219652c, -77, false},
                                        {0x9c40000000000000, -50, false},
                                        {0xe8d4a51000000000, -24, false},
                                        {0xad78ebc5ac620000, 3, false},
                                        {0x813f3978f8940984, 30, false},
                                        {0xc097ce7bc90715b3, 56, false},
                                        {0x8f7e32ce7bea5c70, 83, false},
                                        {0xd5d238a4abe98068, 109, false},
                                        {0x9f4f2726179a2245, 136, false},
                                        {0xed63a231d4c4fb27, 162, false},
                                        {0xb0de65388cc8ada8, 189, false},
                                        {0x83c7088e1aab65db, 216, false},
                                        {0xc45d1df942711d9a, 242, false},
                                        {0x924d692ca61be758, 269, false},
                                        {0xda01ee641a708dea, 295, false},
                                        {0xa26da3999aef774a, 322, false},
                                        {0xf209787bb47d6b85, 348, false},
                                        {0xb454e4a179dd1877, 375, false},
                                        {0x865b86925b9bc5c2, 402, false},
                                        {0xc83553c5c8965d3d, 428, false},
                                        {0x952ab45cfa97a0b3, 455, false},
                                        {0xde469fbd99a05fe3, 481, false},
                                        {0xa59bc234db398c25, 508, false},
                                        {0xf6c69a72a3989f5c, 534, false},
                                        {0xb7dcbf5354e9bece, 561, false},
                                        {0x88fcf317f22241e2, 588, false},
                                        {0xcc20ce9bd35c78a5, 614, false},
                                        {0x98165af37b2153df, 641, false},
                                        {0xe2a0b5dc971f303a, 667, false},
                                        {0xa8d9d1535ce3b396, 694, false},
                                        {0xfb9b7cd9a4a7443c, 720, false},
                                        {0xbb764c4ca7a44410, 747, false},
                                        {0x8bab8eefb6409c1a, 774, false},
                                        {0xd01fef10a657842c, 800, false},
                                        {0x9b10a4e5e9913129, 827, false},
                                        {0xe7109bfba19c0c9d, 853, false},
                                        {0xac2820d9623bf429, 880, false},
                                        {0x80444b5e7aa7cf85, 907, false},
                                        {0xbf21e44003acdd2d, 933, false},
                                        {0x8e679c2f5e44ff8f, 960, false},
                                        {0xd433179d9c8cb841, 986, false},
                                        {0x9e19db92b4e31ba9, 1013, false},
                                        {0xeb96bf6ebadf77d9, 1039, false},
                                        {0xaf87023b9bf0ee6b, 1066, false}};
    uint64pow10 = std::vector<uint64_t>{
            1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
            1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};
    float32info = floatInfo{23, 8, -127};
    float64info = floatInfo{52, 11, -1023};
    isPrint16 = moku::slice<uint16_t>{
            0x0020, 0x007e, 0x00a1, 0x0377, 0x037a, 0x037f, 0x0384, 0x0556,
            0x0559, 0x058a, 0x058d, 0x05c7, 0x05d0, 0x05ea, 0x05f0, 0x05f4,
            0x0606, 0x061b, 0x061e, 0x070d, 0x0710, 0x074a, 0x074d, 0x07b1,
            0x07c0, 0x07fa, 0x0800, 0x082d, 0x0830, 0x085b, 0x085e, 0x085e,
            0x08a0, 0x08bd, 0x08d4, 0x098c, 0x098f, 0x0990, 0x0993, 0x09b2,
            0x09b6, 0x09b9, 0x09bc, 0x09c4, 0x09c7, 0x09c8, 0x09cb, 0x09ce,
            0x09d7, 0x09d7, 0x09dc, 0x09e3, 0x09e6, 0x09fb, 0x0a01, 0x0a0a,
            0x0a0f, 0x0a10, 0x0a13, 0x0a39, 0x0a3c, 0x0a42, 0x0a47, 0x0a48,
            0x0a4b, 0x0a4d, 0x0a51, 0x0a51, 0x0a59, 0x0a5e, 0x0a66, 0x0a75,
            0x0a81, 0x0ab9, 0x0abc, 0x0acd, 0x0ad0, 0x0ad0, 0x0ae0, 0x0ae3,
            0x0ae6, 0x0af1, 0x0af9, 0x0af9, 0x0b01, 0x0b0c, 0x0b0f, 0x0b10,
            0x0b13, 0x0b39, 0x0b3c, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4d,
            0x0b56, 0x0b57, 0x0b5c, 0x0b63, 0x0b66, 0x0b77, 0x0b82, 0x0b8a,
            0x0b8e, 0x0b95, 0x0b99, 0x0b9f, 0x0ba3, 0x0ba4, 0x0ba8, 0x0baa,
            0x0bae, 0x0bb9, 0x0bbe, 0x0bc2, 0x0bc6, 0x0bcd, 0x0bd0, 0x0bd0,
            0x0bd7, 0x0bd7, 0x0be6, 0x0bfa, 0x0c00, 0x0c39, 0x0c3d, 0x0c4d,
            0x0c55, 0x0c5a, 0x0c60, 0x0c63, 0x0c66, 0x0c6f, 0x0c78, 0x0cb9,
            0x0cbc, 0x0ccd, 0x0cd5, 0x0cd6, 0x0cde, 0x0ce3, 0x0ce6, 0x0cf2,
            0x0d01, 0x0d3a, 0x0d3d, 0x0d4f, 0x0d54, 0x0d63, 0x0d66, 0x0d7f,
            0x0d82, 0x0d96, 0x0d9a, 0x0dbd, 0x0dc0, 0x0dc6, 0x0dca, 0x0dca,
            0x0dcf, 0x0ddf, 0x0de6, 0x0def, 0x0df2, 0x0df4, 0x0e01, 0x0e3a,
            0x0e3f, 0x0e5b, 0x0e81, 0x0e84, 0x0e87, 0x0e8a, 0x0e8d, 0x0e8d,
            0x0e94, 0x0ea7, 0x0eaa, 0x0ebd, 0x0ec0, 0x0ecd, 0x0ed0, 0x0ed9,
            0x0edc, 0x0edf, 0x0f00, 0x0f6c, 0x0f71, 0x0fda, 0x1000, 0x10c7,
            0x10cd, 0x10cd, 0x10d0, 0x124d, 0x1250, 0x125d, 0x1260, 0x128d,
            0x1290, 0x12b5, 0x12b8, 0x12c5, 0x12c8, 0x1315, 0x1318, 0x135a,
            0x135d, 0x137c, 0x1380, 0x1399, 0x13a0, 0x13f5, 0x13f8, 0x13fd,
            0x1400, 0x169c, 0x16a0, 0x16f8, 0x1700, 0x1714, 0x1720, 0x1736,
            0x1740, 0x1753, 0x1760, 0x1773, 0x1780, 0x17dd, 0x17e0, 0x17e9,
            0x17f0, 0x17f9, 0x1800, 0x180d, 0x1810, 0x1819, 0x1820, 0x1877,
            0x1880, 0x18aa, 0x18b0, 0x18f5, 0x1900, 0x192b, 0x1930, 0x193b,
            0x1940, 0x1940, 0x1944, 0x196d, 0x1970, 0x1974, 0x1980, 0x19ab,
            0x19b0, 0x19c9, 0x19d0, 0x19da, 0x19de, 0x1a1b, 0x1a1e, 0x1a7c,
            0x1a7f, 0x1a89, 0x1a90, 0x1a99, 0x1aa0, 0x1aad, 0x1ab0, 0x1abe,
            0x1b00, 0x1b4b, 0x1b50, 0x1b7c, 0x1b80, 0x1bf3, 0x1bfc, 0x1c37,
            0x1c3b, 0x1c49, 0x1c4d, 0x1c88, 0x1cc0, 0x1cc7, 0x1cd0, 0x1cf9,
            0x1d00, 0x1df5, 0x1dfb, 0x1f15, 0x1f18, 0x1f1d, 0x1f20, 0x1f45,
            0x1f48, 0x1f4d, 0x1f50, 0x1f7d, 0x1f80, 0x1fd3, 0x1fd6, 0x1fef,
            0x1ff2, 0x1ffe, 0x2010, 0x2027, 0x2030, 0x205e, 0x2070, 0x2071,
            0x2074, 0x209c, 0x20a0, 0x20be, 0x20d0, 0x20f0, 0x2100, 0x218b,
            0x2190, 0x2426, 0x2440, 0x244a, 0x2460, 0x2b73, 0x2b76, 0x2b95,
            0x2b98, 0x2bb9, 0x2bbd, 0x2bd1, 0x2bec, 0x2bef, 0x2c00, 0x2cf3,
            0x2cf9, 0x2d27, 0x2d2d, 0x2d2d, 0x2d30, 0x2d67, 0x2d6f, 0x2d70,
            0x2d7f, 0x2d96, 0x2da0, 0x2e44, 0x2e80, 0x2ef3, 0x2f00, 0x2fd5,
            0x2ff0, 0x2ffb, 0x3001, 0x3096, 0x3099, 0x30ff, 0x3105, 0x312d,
            0x3131, 0x31ba, 0x31c0, 0x31e3, 0x31f0, 0x4db5, 0x4dc0, 0x9fd5,
            0xa000, 0xa48c, 0xa490, 0xa4c6, 0xa4d0, 0xa62b, 0xa640, 0xa6f7,
            0xa700, 0xa7b7, 0xa7f7, 0xa82b, 0xa830, 0xa839, 0xa840, 0xa877,
            0xa880, 0xa8c5, 0xa8ce, 0xa8d9, 0xa8e0, 0xa8fd, 0xa900, 0xa953,
            0xa95f, 0xa97c, 0xa980, 0xa9d9, 0xa9de, 0xaa36, 0xaa40, 0xaa4d,
            0xaa50, 0xaa59, 0xaa5c, 0xaac2, 0xaadb, 0xaaf6, 0xab01, 0xab06,
            0xab09, 0xab0e, 0xab11, 0xab16, 0xab20, 0xab65, 0xab70, 0xabed,
            0xabf0, 0xabf9, 0xac00, 0xd7a3, 0xd7b0, 0xd7c6, 0xd7cb, 0xd7fb,
            0xf900, 0xfa6d, 0xfa70, 0xfad9, 0xfb00, 0xfb06, 0xfb13, 0xfb17,
            0xfb1d, 0xfbc1, 0xfbd3, 0xfd3f, 0xfd50, 0xfd8f, 0xfd92, 0xfdc7,
            0xfdf0, 0xfdfd, 0xfe00, 0xfe19, 0xfe20, 0xfe6b, 0xfe70, 0xfefc,
            0xff01, 0xffbe, 0xffc2, 0xffc7, 0xffca, 0xffcf, 0xffd2, 0xffd7,
            0xffda, 0xffdc, 0xffe0, 0xffee, 0xfffc, 0xfffd};
    isNotPrint16 = moku::slice<uint16_t>{
            0x00ad, 0x038b, 0x038d, 0x03a2, 0x0530, 0x0560, 0x0588, 0x0590,
            0x06dd, 0x083f, 0x08b5, 0x08e2, 0x0984, 0x09a9, 0x09b1, 0x09de,
            0x0a04, 0x0a29, 0x0a31, 0x0a34, 0x0a37, 0x0a3d, 0x0a5d, 0x0a84,
            0x0a8e, 0x0a92, 0x0aa9, 0x0ab1, 0x0ab4, 0x0ac6, 0x0aca, 0x0b04,
            0x0b29, 0x0b31, 0x0b34, 0x0b5e, 0x0b84, 0x0b91, 0x0b9b, 0x0b9d,
            0x0bc9, 0x0c04, 0x0c0d, 0x0c11, 0x0c29, 0x0c45, 0x0c49, 0x0c57,
            0x0c84, 0x0c8d, 0x0c91, 0x0ca9, 0x0cb4, 0x0cc5, 0x0cc9, 0x0cdf,
            0x0cf0, 0x0d04, 0x0d0d, 0x0d11, 0x0d45, 0x0d49, 0x0d84, 0x0db2,
            0x0dbc, 0x0dd5, 0x0dd7, 0x0e83, 0x0e89, 0x0e98, 0x0ea0, 0x0ea4,
            0x0ea6, 0x0eac, 0x0eba, 0x0ec5, 0x0ec7, 0x0f48, 0x0f98, 0x0fbd,
            0x0fcd, 0x10c6, 0x1249, 0x1257, 0x1259, 0x1289, 0x12b1, 0x12bf,
            0x12c1, 0x12d7, 0x1311, 0x1680, 0x170d, 0x176d, 0x1771, 0x191f,
            0x1a5f, 0x1cf7, 0x1f58, 0x1f5a, 0x1f5c, 0x1f5e, 0x1fb5, 0x1fc5,
            0x1fdc, 0x1ff5, 0x208f, 0x23ff, 0x2bc9, 0x2c2f, 0x2c5f, 0x2d26,
            0x2da7, 0x2daf, 0x2db7, 0x2dbf, 0x2dc7, 0x2dcf, 0x2dd7, 0x2ddf,
            0x2e9a, 0x3040, 0x318f, 0x321f, 0x32ff, 0xa7af, 0xa9ce, 0xa9ff,
            0xab27, 0xab2f, 0xfb37, 0xfb3d, 0xfb3f, 0xfb42, 0xfb45, 0xfe53,
            0xfe67, 0xfe75, 0xffe7};
    isPrint32 = moku::slice<uint32_t>{
            0x010000, 0x01004d, 0x010050, 0x01005d, 0x010080, 0x0100fa,
            0x010100, 0x010102, 0x010107, 0x010133, 0x010137, 0x01019b,
            0x0101a0, 0x0101a0, 0x0101d0, 0x0101fd, 0x010280, 0x01029c,
            0x0102a0, 0x0102d0, 0x0102e0, 0x0102fb, 0x010300, 0x010323,
            0x010330, 0x01034a, 0x010350, 0x01037a, 0x010380, 0x0103c3,
            0x0103c8, 0x0103d5, 0x010400, 0x01049d, 0x0104a0, 0x0104a9,
            0x0104b0, 0x0104d3, 0x0104d8, 0x0104fb, 0x010500, 0x010527,
            0x010530, 0x010563, 0x01056f, 0x01056f, 0x010600, 0x010736,
            0x010740, 0x010755, 0x010760, 0x010767, 0x010800, 0x010805,
            0x010808, 0x010838, 0x01083c, 0x01083c, 0x01083f, 0x01089e,
            0x0108a7, 0x0108af, 0x0108e0, 0x0108f5, 0x0108fb, 0x01091b,
            0x01091f, 0x010939, 0x01093f, 0x01093f, 0x010980, 0x0109b7,
            0x0109bc, 0x0109cf, 0x0109d2, 0x010a06, 0x010a0c, 0x010a33,
            0x010a38, 0x010a3a, 0x010a3f, 0x010a47, 0x010a50, 0x010a58,
            0x010a60, 0x010a9f, 0x010ac0, 0x010ae6, 0x010aeb, 0x010af6,
            0x010b00, 0x010b35, 0x010b39, 0x010b55, 0x010b58, 0x010b72,
            0x010b78, 0x010b91, 0x010b99, 0x010b9c, 0x010ba9, 0x010baf,
            0x010c00, 0x010c48, 0x010c80, 0x010cb2, 0x010cc0, 0x010cf2,
            0x010cfa, 0x010cff, 0x010e60, 0x010e7e, 0x011000, 0x01104d,
            0x011052, 0x01106f, 0x01107f, 0x0110c1, 0x0110d0, 0x0110e8,
            0x0110f0, 0x0110f9, 0x011100, 0x011143, 0x011150, 0x011176,
            0x011180, 0x0111cd, 0x0111d0, 0x0111f4, 0x011200, 0x01123e,
            0x011280, 0x0112a9, 0x0112b0, 0x0112ea, 0x0112f0, 0x0112f9,
            0x011300, 0x01130c, 0x01130f, 0x011310, 0x011313, 0x011339,
            0x01133c, 0x011344, 0x011347, 0x011348, 0x01134b, 0x01134d,
            0x011350, 0x011350, 0x011357, 0x011357, 0x01135d, 0x011363,
            0x011366, 0x01136c, 0x011370, 0x011374, 0x011400, 0x01145d,
            0x011480, 0x0114c7, 0x0114d0, 0x0114d9, 0x011580, 0x0115b5,
            0x0115b8, 0x0115dd, 0x011600, 0x011644, 0x011650, 0x011659,
            0x011660, 0x01166c, 0x011680, 0x0116b7, 0x0116c0, 0x0116c9,
            0x011700, 0x011719, 0x01171d, 0x01172b, 0x011730, 0x01173f,
            0x0118a0, 0x0118f2, 0x0118ff, 0x0118ff, 0x011ac0, 0x011af8,
            0x011c00, 0x011c45, 0x011c50, 0x011c6c, 0x011c70, 0x011c8f,
            0x011c92, 0x011cb6, 0x012000, 0x012399, 0x012400, 0x012474,
            0x012480, 0x012543, 0x013000, 0x01342e, 0x014400, 0x014646,
            0x016800, 0x016a38, 0x016a40, 0x016a69, 0x016a6e, 0x016a6f,
            0x016ad0, 0x016aed, 0x016af0, 0x016af5, 0x016b00, 0x016b45,
            0x016b50, 0x016b77, 0x016b7d, 0x016b8f, 0x016f00, 0x016f44,
            0x016f50, 0x016f7e, 0x016f8f, 0x016f9f, 0x016fe0, 0x016fe0,
            0x017000, 0x0187ec, 0x018800, 0x018af2, 0x01b000, 0x01b001,
            0x01bc00, 0x01bc6a, 0x01bc70, 0x01bc7c, 0x01bc80, 0x01bc88,
            0x01bc90, 0x01bc99, 0x01bc9c, 0x01bc9f, 0x01d000, 0x01d0f5,
            0x01d100, 0x01d126, 0x01d129, 0x01d172, 0x01d17b, 0x01d1e8,
            0x01d200, 0x01d245, 0x01d300, 0x01d356, 0x01d360, 0x01d371,
            0x01d400, 0x01d49f, 0x01d4a2, 0x01d4a2, 0x01d4a5, 0x01d4a6,
            0x01d4a9, 0x01d50a, 0x01d50d, 0x01d546, 0x01d54a, 0x01d6a5,
            0x01d6a8, 0x01d7cb, 0x01d7ce, 0x01da8b, 0x01da9b, 0x01daaf,
            0x01e000, 0x01e018, 0x01e01b, 0x01e02a, 0x01e800, 0x01e8c4,
            0x01e8c7, 0x01e8d6, 0x01e900, 0x01e94a, 0x01e950, 0x01e959,
            0x01e95e, 0x01e95f, 0x01ee00, 0x01ee24, 0x01ee27, 0x01ee3b,
            0x01ee42, 0x01ee42, 0x01ee47, 0x01ee54, 0x01ee57, 0x01ee64,
            0x01ee67, 0x01ee9b, 0x01eea1, 0x01eebb, 0x01eef0, 0x01eef1,
            0x01f000, 0x01f02b, 0x01f030, 0x01f093, 0x01f0a0, 0x01f0ae,
            0x01f0b1, 0x01f0f5, 0x01f100, 0x01f10c, 0x01f110, 0x01f16b,
            0x01f170, 0x01f1ac, 0x01f1e6, 0x01f202, 0x01f210, 0x01f23b,
            0x01f240, 0x01f248, 0x01f250, 0x01f251, 0x01f300, 0x01f6d2,
            0x01f6e0, 0x01f6ec, 0x01f6f0, 0x01f6f6, 0x01f700, 0x01f773,
            0x01f780, 0x01f7d4, 0x01f800, 0x01f80b, 0x01f810, 0x01f847,
            0x01f850, 0x01f859, 0x01f860, 0x01f887, 0x01f890, 0x01f8ad,
            0x01f910, 0x01f927, 0x01f930, 0x01f930, 0x01f933, 0x01f94b,
            0x01f950, 0x01f95e, 0x01f980, 0x01f991, 0x01f9c0, 0x01f9c0,
            0x020000, 0x02a6d6, 0x02a700, 0x02b734, 0x02b740, 0x02b81d,
            0x02b820, 0x02cea1, 0x02f800, 0x02fa1d, 0x0e0100, 0x0e01ef};
    isNotPrint32 = moku::slice<uint16_t>{
            0x000c, 0x0027, 0x003b, 0x003e, 0x018f, 0x039e, 0x0809, 0x0836,
            0x0856, 0x08f3, 0x0a04, 0x0a14, 0x0a18, 0x10bd, 0x1135, 0x11e0,
            0x1212, 0x1287, 0x1289, 0x128e, 0x129e, 0x1304, 0x1329, 0x1331,
            0x1334, 0x145a, 0x145c, 0x1c09, 0x1c37, 0x1ca8, 0x246f, 0x6a5f,
            0x6b5a, 0x6b62, 0xd455, 0xd49d, 0xd4ad, 0xd4ba, 0xd4bc, 0xd4c4,
            0xd506, 0xd515, 0xd51d, 0xd53a, 0xd53f, 0xd545, 0xd551, 0xdaa0,
            0xe007, 0xe022, 0xe025, 0xee04, 0xee20, 0xee23, 0xee28, 0xee33,
            0xee38, 0xee3a, 0xee48, 0xee4a, 0xee4c, 0xee50, 0xee53, 0xee58,
            0xee5a, 0xee5c, 0xee5e, 0xee60, 0xee63, 0xee6b, 0xee73, 0xee78,
            0xee7d, 0xee7f, 0xee8a, 0xeea4, 0xeeaa, 0xf0c0, 0xf0d0, 0xf12f,
            0xf91f, 0xf93f};
    isGraphic = moku::slice<uint16_t>{0x00a0,
                                      0x1680,
                                      0x2000,
                                      0x2001,
                                      0x2002,
                                      0x2003,
                                      0x2004,
                                      0x2005,
                                      0x2006,
                                      0x2007,
                                      0x2008,
                                      0x2009,
                                      0x200a,
                                      0x202f,
                                      0x205f,
                                      0x3000};
    shifts = std::vector<unsigned int>{
            {1 << 1, 1}, {1 << 2, 2}, {1 << 3, 3}, {1 << 4, 4}, {1 << 5, 5}};
    _main();
    return 0;
}
