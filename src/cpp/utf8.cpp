bool FullRune(moku::slice<uint8_t> p)
{
    acceptRange accept{};
    int n{0};
    uint8_t x{0};

    n = len(p);
    if (n == 0) {
        return false;
    }
    x = first[p[0]];
    if (n >= int(x & 7)) {
        return true;
    }
    accept = acceptRanges[x >> 4];
    if (n > 1) {
        {
            uint8_t c{0};

            c = p[1];

            if (c < accept.lo || accept.hi < c) {
                return true;
            } else {
                if (n > 2 && (p[2] < locb || hicb < p[2])) {
                    return true;
                }
            }
        }
    }
    return false;
}
bool FullRuneInString(std::string s)
{
    acceptRange accept{};
    int n{0};
    uint8_t x{0};

    n = len(s);
    if (n == 0) {
        return false;
    }
    x = first[s[0]];
    if (n >= int(x & 7)) {
        return true;
    }
    accept = acceptRanges[x >> 4];
    if (n > 1) {
        {
            uint8_t c{0};

            c = s[1];

            if (c < accept.lo || accept.hi < c) {
                return true;
            } else {
                if (n > 2 && (s[2] < locb || hicb < s[2])) {
                    return true;
                }
            }
        }
    }
    return false;
}
std::tuple<int32_t, int> DecodeRune(moku::slice<uint8_t> p)
{
    acceptRange accept{};
    uint8_t b1{0};
    uint8_t b2{0};
    uint8_t b3{0};
    int n{0};
    uint8_t p0{0};
    int32_t r{0};
    int size{0};
    uint8_t sz{0};
    uint8_t x{0};

    n = len(p);
    if (n < 1) {
        return {RuneError, 0};
    }
    p0 = p[0];
    x = first[p0];
    if (x >= as) {
        mask = rune(x) << 31 >> 31;
        return {rune(p[0]) & ~(mask) | RuneError & mask, 1};
    }
    sz = x & 7;
    accept = acceptRanges[x >> 4];
    if (n < int(sz)) {
        return {RuneError, 1};
    }
    b1 = p[1];
    if (b1 < accept.lo || accept.hi < b1) {
        return {RuneError, 1};
    }
    if (sz == 2) {
        return {rune(p0 & mask2) << 6 | rune(b1 & maskx), 2};
    }
    b2 = p[2];
    if (b2 < locb || hicb < b2) {
        return {RuneError, 1};
    }
    if (sz == 3) {
        return {rune(p0 & mask3) << 12 | rune(b1 & maskx) << 6 |
                        rune(b2 & maskx),
                3};
    }
    b3 = p[3];
    if (b3 < locb || hicb < b3) {
        return {RuneError, 1};
    }
    return {rune(p0 & mask4) << 18 | rune(b1 & maskx) << 12 |
                    rune(b2 & maskx) << 6 | rune(b3 & maskx),
            4};
}
std::tuple<int32_t, int> DecodeRuneInString(std::string s)
{
    acceptRange accept{};
    int n{0};
    int32_t r{0};
    uint8_t s0{0};
    uint8_t s1{0};
    uint8_t s2{0};
    uint8_t s3{0};
    int size{0};
    uint8_t sz{0};
    uint8_t x{0};

    n = len(s);
    if (n < 1) {
        return {RuneError, 0};
    }
    s0 = s[0];
    x = first[s0];
    if (x >= as) {
        mask = rune(x) << 31 >> 31;
        return {rune(s[0]) & ~(mask) | RuneError & mask, 1};
    }
    sz = x & 7;
    accept = acceptRanges[x >> 4];
    if (n < int(sz)) {
        return {RuneError, 1};
    }
    s1 = s[1];
    if (s1 < accept.lo || accept.hi < s1) {
        return {RuneError, 1};
    }
    if (sz == 2) {
        return {rune(s0 & mask2) << 6 | rune(s1 & maskx), 2};
    }
    s2 = s[2];
    if (s2 < locb || hicb < s2) {
        return {RuneError, 1};
    }
    if (sz == 3) {
        return {rune(s0 & mask3) << 12 | rune(s1 & maskx) << 6 |
                        rune(s2 & maskx),
                3};
    }
    s3 = s[3];
    if (s3 < locb || hicb < s3) {
        return {RuneError, 1};
    }
    return {rune(s0 & mask4) << 18 | rune(s1 & maskx) << 12 |
                    rune(s2 & maskx) << 6 | rune(s3 & maskx),
            4};
}
std::tuple<int32_t, int> DecodeLastRune(moku::slice<uint8_t> p)
{
    int end{0};
    int lim{0};
    int32_t r{0};
    int size{0};
    int start{0};

    end = len(p);
    if (end == 0) {
        return {RuneError, 0};
    }
    start = end - 1;
    r = rune(p[start]);
    if (r < RuneSelf) {
        return {r, 1};
    }
    lim = end - UTFMax;
    if (lim < 0) {
        lim = 0;
    }
    for (start--; start >= lim; start--) {
        if (RuneStart(p[start])) {
            break;
        }
    }
    if (start < 0) {
        start = 0;
    }
    std::tie(r, size) =
            DecodeRune(moku::slice_expr<moku::slice<uint8_t>>(p, start, end));
    if (start + size != end) {
        return {RuneError, 1};
    }
    return {r, size};
}
std::tuple<int32_t, int> DecodeLastRuneInString(std::string s)
{
    int end{0};
    int lim{0};
    int32_t r{0};
    int size{0};
    int start{0};

    end = len(s);
    if (end == 0) {
        return {RuneError, 0};
    }
    start = end - 1;
    r = rune(s[start]);
    if (r < RuneSelf) {
        return {r, 1};
    }
    lim = end - UTFMax;
    if (lim < 0) {
        lim = 0;
    }
    for (start--; start >= lim; start--) {
        if (RuneStart(s[start])) {
            break;
        }
    }
    if (start < 0) {
        start = 0;
    }
    std::tie(r, size) =
            DecodeRuneInString(moku::slice_expr<std::string>(s, start, end));
    if (start + size != end) {
        return {RuneError, 1};
    }
    return {r, size};
}
int RuneLen(int32_t r)
{
    if ((r < 0)) {
    _ident_0_:
        return -1;
    } else if ((r <= rune1Max)) {
    _ident_1_:
        return 1;
    } else if ((r <= rune2Max)) {
    _ident_2_:
        return 2;
    } else if ((surrogateMin <= r && r <= surrogateMax)) {
    _ident_3_:
        return -1;
    } else if ((r <= rune3Max)) {
    _ident_4_:
        return 3;
    } else if ((r <= MaxRune)) {
    _ident_5_:
        return 4;
    }
    return -1;
}
int EncodeRune(moku::slice<uint8_t> p, int32_t r)
{
    {
        uint32_t i{0};
        i = uint32_t(r);
        if ((i <= rune1Max)) {
        _ident_6_:
            p[0] = byte(r);
            return 1;
        } else if ((i <= rune2Max)) {
        _ident_7_:
            _ = p[1];
            p[0] = t2 | byte(r >> 6);
            p[1] = tx | byte(r) & maskx;
            return 2;
        } else if ((i > MaxRune) || (surrogateMin <= i && i <= surrogateMax)) {
        _ident_8_:
            r = RuneError;
            goto _ident_9_;
        } else if ((i <= rune3Max)) {
        _ident_9_:
            _ = p[2];
            p[0] = t3 | byte(r >> 12);
            p[1] = tx | byte(r >> 6) & maskx;
            p[2] = tx | byte(r) & maskx;
            return 3;
        } else {
        _ident_10_:
            _ = p[3];
            p[0] = t4 | byte(r >> 18);
            p[1] = tx | byte(r >> 12) & maskx;
            p[2] = tx | byte(r >> 6) & maskx;
            p[3] = tx | byte(r) & maskx;
            return 4;
        }
    }
}
int RuneCount(moku::slice<uint8_t> p)
{
    int n{0};
    int np{0};

    np = len(p);
    {
        int i{0};
        for (i = 0; i < np;) {
            acceptRange accept{};
            uint8_t c{0};
            int size{0};
            uint8_t x{0};

            n++;
            c = p[i];
            if (c < RuneSelf) {
                i++;
                continue;
            }
            x = first[c];
            if (x == xx) {
                i++;
                continue;
            }
            size = int(x & 7);
            if (i + size > np) {
                i++;
                continue;
            }
            accept = acceptRanges[x >> 4];
            {
                uint8_t c{0};

                c = p[i + 1];

                if (c < accept.lo || accept.hi < c) {
                    size = 1;
                } else {
                    if (size == 2) {
                    } else {
                        {
                            uint8_t c{0};

                            c = p[i + 2];

                            if (c < locb || hicb < c) {
                                size = 1;
                            } else {
                                if (size == 3) {
                                } else {
                                    {
                                        uint8_t c{0};

                                        c = p[i + 3];

                                        if (c < locb || hicb < c) {
                                            size = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            i += size;
        }
    }
    return n;
}
int RuneCountInString(std::string s)
{
    int n{0};
    int ns{0};

    ns = len(s);
    {
        int i{0};
        for (i = 0; i < ns; n++) {
            acceptRange accept{};
            uint8_t c{0};
            int size{0};
            uint8_t x{0};

            c = s[i];
            if (c < RuneSelf) {
                i++;
                continue;
            }
            x = first[c];
            if (x == xx) {
                i++;
                continue;
            }
            size = int(x & 7);
            if (i + size > ns) {
                i++;
                continue;
            }
            accept = acceptRanges[x >> 4];
            {
                uint8_t c{0};

                c = s[i + 1];

                if (c < accept.lo || accept.hi < c) {
                    size = 1;
                } else {
                    if (size == 2) {
                    } else {
                        {
                            uint8_t c{0};

                            c = s[i + 2];

                            if (c < locb || hicb < c) {
                                size = 1;
                            } else {
                                if (size == 3) {
                                } else {
                                    {
                                        uint8_t c{0};

                                        c = s[i + 3];

                                        if (c < locb || hicb < c) {
                                            size = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            i += size;
        }
    }
    return n;
}
bool RuneStart(uint8_t b)
{
    return b & 0xC0 != 0x80;
}
bool Valid(moku::slice<uint8_t> p)
{
    int n{0};

    n = len(p);
    {
        int i{0};
        for (i = 0; i < n;) {
            acceptRange accept{};
            uint8_t pi{0};
            int size{0};
            uint8_t x{0};

            pi = p[i];
            if (pi < RuneSelf) {
                i++;
                continue;
            }
            x = first[pi];
            if (x == xx) {
                return false;
            }
            size = int(x & 7);
            if (i + size > n) {
                return false;
            }
            accept = acceptRanges[x >> 4];
            {
                uint8_t c{0};

                c = p[i + 1];

                if (c < accept.lo || accept.hi < c) {
                    return false;
                } else {
                    if (size == 2) {
                    } else {
                        {
                            uint8_t c{0};

                            c = p[i + 2];

                            if (c < locb || hicb < c) {
                                return false;
                            } else {
                                if (size == 3) {
                                } else {
                                    {
                                        uint8_t c{0};

                                        c = p[i + 3];

                                        if (c < locb || hicb < c) {
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            i += size;
        }
    }
    return true;
}
bool ValidString(std::string s)
{
    int n{0};

    n = len(s);
    {
        int i{0};
        for (i = 0; i < n;) {
            acceptRange accept{};
            uint8_t si{0};
            int size{0};
            uint8_t x{0};

            si = s[i];
            if (si < RuneSelf) {
                i++;
                continue;
            }
            x = first[si];
            if (x == xx) {
                return false;
            }
            size = int(x & 7);
            if (i + size > n) {
                return false;
            }
            accept = acceptRanges[x >> 4];
            {
                uint8_t c{0};

                c = s[i + 1];

                if (c < accept.lo || accept.hi < c) {
                    return false;
                } else {
                    if (size == 2) {
                    } else {
                        {
                            uint8_t c{0};

                            c = s[i + 2];

                            if (c < locb || hicb < c) {
                                return false;
                            } else {
                                if (size == 3) {
                                } else {
                                    {
                                        uint8_t c{0};

                                        c = s[i + 3];

                                        if (c < locb || hicb < c) {
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            i += size;
        }
    }
    return true;
}
bool ValidRune(int32_t r)
{
    if ((0 <= r && r < surrogateMin)) {
    _ident_11_:
        return true;
    } else if ((surrogateMax < r && r <= MaxRune)) {
    _ident_12_:
        return true;
    }
    return false;
}
int main()
{
    first = std::vector<uint8_t>{
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as,
            xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx,
            xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx,
            xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx,
            xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx,
            xx, xx, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1,
            s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1,
            s2, s3, s3, s3, s3, s3, s3, s3, s3, s3, s3, s3, s3, s4, s3, s3,
            s5, s6, s6, s6, s7, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx};
    acceptRanges = std::vector<acceptRange>{{0, {locb, hicb}},
                                            {1, {0xA0, hicb}},
                                            {2, {locb, 0x9F}},
                                            {3, {0x90, hicb}},
                                            {4, {locb, 0x8F}}};
    _main();
    return 0;
}
