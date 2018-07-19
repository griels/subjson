std::string KeySizeError::_ErrorByValue()
{
    return "crypto/rc4: invalid key size " + strconv::Itoa(int(this));
}
std::tuple<Cipher *, moku::error> NewCipher(moku::slice<uint8_t> key)
{
    Cipher c{};
    uint8_t j{0};
    int k{0};

    k = len(key);
    if (k < 1 || k > 256) {
        return {nil, KeySizeError(k)};
    }
    {
        int i{0};
        for (i = 0; i < 256; i++) {
            c.s[i] = uint32_t(i);
        }
    }
    {
        int i{0};
        for (i = 0; i < 256; i++) {
            j += uint8_t(c.s[i]) + key[i % k];
            std::tie(c.s[i], c.s[j]) =
                    std::tuple<uint32_t, uint32_t>(c.s[j], c.s[i]);
        }
    }
    return {&c, nil};
}
void Cipher::Reset()
{
    {
        int i{0};
        for (i : moku::range_key<std::vector<uint32_t>>(this->s)) {
            this->s[i] = 0;
        }
    }
    std::tie(this->i, this->j) = std::tuple<uint8_t, uint8_t>(0, 0);
}
void Cipher::xorKeyStreamGeneric(moku::slice<uint8_t> dst,
                                 moku::slice<uint8_t> src)
{
    uint8_t i{0};
    uint8_t j{0};

    std::tie(i, j) = std::tuple<uint8_t, uint8_t>(this->i, this->j);
    {
        int k{0};
        uint8_t v{0};
        for (std::tie(k, v) :
             moku::range_key_value<moku::slice<uint8_t>>(src)) {
            i += 1;
            j += uint8_t(this->s[i]);
            std::tie(this->s[i], this->s[j]) =
                    std::tuple<uint32_t, uint32_t>(this->s[j], this->s[i]);
            dst[k] = v ^ uint8_t(this->s[uint8_t(this->s[i] + this->s[j])]);
        }
    }
    std::tie(this->i, this->j) = std::tuple<uint8_t, uint8_t>(i, j);
}
int main()
{
    _main();
    return 0;
}
void xorKeyStream(uint8_t *dst,
                  uint8_t *src,
                  int n,
                  std::vector<uint32_t> *state,
                  uint8_t *i,
                  uint8_t *j)
{
}
void Cipher::XORKeyStream(moku::slice<uint8_t> dst, moku::slice<uint8_t> src)
{
    if (len(src) == 0) {
        return;
    }
    xorKeyStream(&dst[0], &src[0], len(src), &this->s, &this->i, &this->j);
}
int main()
{
    _main();
    return 0;
}
