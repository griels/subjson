#pragma once
#include "bytes.h"
#include "errors.h"
#include "fmt.h"
#include "io.h"
namespace hex {
std::tuple<int, moku::error> Decode(moku::slice<uint8_t> dst,
                                    moku::slice<uint8_t> src);
std::tuple<moku::slice<uint8_t>, moku::error> DecodeString(std::string s);
int DecodedLen(int x);
std::string Dump(moku::slice<uint8_t> data);
WriteCloser Dumper(Writer w);
int Encode(moku::slice<uint8_t> dst, moku::slice<uint8_t> src);
std::string EncodeToString(moku::slice<uint8_t> src);
int EncodedLen(int n);
moku::error ErrLength;

struct InvalidByteError : public moku::basic<uint8_t> {
    InvalidByteError() : moku::basic<uint8_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<InvalidByteError>(
        const InvalidByteError &invalidbyteerror)
{
    return moku::is_nil<uint8_t>(uint8_t(invalidbyteerror));
}
inline std::string Error()
{
    InvalidByteError invalidbyteerror = *this;
    return invalidbyteerror._ErrorByValue();
}
std::string _ErrorByValue();

struct dumper {
    Writer w;
    std::vector<uint8_t> rightChars;
    std::vector<uint8_t> buf;
    int used{0};
    unsigned int n{0};
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> data);
    moku::error Close();
};
template <>
inline bool moku::is_nil<dumper>(const dumper &dumper)
{
    return moku::is_nil<Writer>(dumper.w) &&
           moku::is_nil<std::vector<uint8_t>>(dumper.rightChars) &&
           moku::is_nil<std::vector<uint8_t>>(dumper.buf) &&
           moku::is_nil<int>(dumper.used) &&
           moku::is_nil<unsigned int>(dumper.n);
}
std::tuple<uint8_t, bool> fromHexChar(uint8_t c);
static constexpr std::string hextable{"0123456789abcdef"};
uint8_t toChar(uint8_t b);
} // namespace hex
