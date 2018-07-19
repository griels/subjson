#pragma once
#include "strconv.h"
namespace rc4 {

struct Cipher {
    std::vector<uint32_t> s;
    uint8_t i{0};
    uint8_t j{0};
    void Reset();
    void xorKeyStreamGeneric(moku::slice<uint8_t> dst,
                             moku::slice<uint8_t> src);
    void XORKeyStream(moku::slice<uint8_t> dst, moku::slice<uint8_t> src);
};
template <>
inline bool moku::is_nil<Cipher>(const Cipher &cipher)
{
    return moku::is_nil<std::vector<uint32_t>>(cipher.s) &&
           moku::is_nil<uint8_t>(cipher.i) && moku::is_nil<uint8_t>(cipher.j);
}
struct KeySizeError : public moku::basic<int> {
    KeySizeError() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<KeySizeError>(const KeySizeError &keysizeerror)
{
    return moku::is_nil<int>(int(keysizeerror));
}
inline std::string Error()
{
    KeySizeError keysizeerror = *this;
    return keysizeerror._ErrorByValue();
}
std::string _ErrorByValue();
std::tuple<Cipher *, moku::error> NewCipher(moku::slice<uint8_t> key);
void xorKeyStream(uint8_t *dst,
                  uint8_t *src,
                  int n,
                  std::vector<uint32_t> *state,
                  uint8_t *i,
                  uint8_t *j);
} // namespace rc4
