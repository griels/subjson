#pragma once
#include "errors.h"
#include "math.h"
#include "utf8.h"
namespace strconv {
moku::slice<uint8_t> AppendBool(moku::slice<uint8_t> dst, bool b);
moku::slice<uint8_t> AppendFloat(
        moku::slice<uint8_t> dst, double f, uint8_t fmt, int prec, int bitSize);
moku::slice<uint8_t> AppendInt(moku::slice<uint8_t> dst, int64_t i, int base);
moku::slice<uint8_t> AppendQuote(moku::slice<uint8_t> dst, std::string s);
moku::slice<uint8_t> AppendQuoteRune(moku::slice<uint8_t> dst, int32_t r);
moku::slice<uint8_t> AppendQuoteRuneToASCII(moku::slice<uint8_t> dst,
                                            int32_t r);
moku::slice<uint8_t> AppendQuoteRuneToGraphic(moku::slice<uint8_t> dst,
                                              int32_t r);
moku::slice<uint8_t> AppendQuoteToASCII(moku::slice<uint8_t> dst,
                                        std::string s);
moku::slice<uint8_t> AppendQuoteToGraphic(moku::slice<uint8_t> dst,
                                          std::string s);
moku::slice<uint8_t> AppendUint(moku::slice<uint8_t> dst, uint64_t i, int base);
std::tuple<int, moku::error> Atoi(std::string s);
bool CanBackquote(std::string s);
moku::error ErrRange;
moku::error ErrSyntax;
std::string FormatBool(bool b);
std::string FormatFloat(double f, uint8_t fmt, int prec, int bitSize);
std::string FormatInt(int64_t i, int base);
std::string FormatUint(uint64_t i, int base);
constexpr int IntSize{64};
bool IsGraphic(int32_t r);
bool IsPrint(int32_t r);
std::string Itoa(int i);

struct NumError {
    std::string Func{""};
    std::string Num{""};
    moku::error Err;
    std::string Error();
};
template <>
inline bool moku::is_nil<NumError>(const NumError &numerror)
{
    return moku::is_nil<std::string>(numerror.Func) &&
           moku::is_nil<std::string>(numerror.Num) &&
           moku::is_nil<moku::error>(numerror.Err);
}
std::tuple<bool, moku::error> ParseBool(std::string str);
std::tuple<double, moku::error> ParseFloat(std::string s, int bitSize);
std::tuple<int64_t, moku::error> ParseInt(std::string s, int base, int bitSize);
std::tuple<uint64_t, moku::error> ParseUint(std::string s,
                                            int base,
                                            int bitSize);
std::string Quote(std::string s);
std::string QuoteRune(int32_t r);
std::string QuoteRuneToASCII(int32_t r);
std::string QuoteRuneToGraphic(int32_t r);
std::string QuoteToASCII(std::string s);
std::string QuoteToGraphic(std::string s);
std::tuple<std::string, moku::error> Unquote(std::string s);
std::tuple<int32_t, bool, std::string, moku::error> UnquoteChar(std::string s,
                                                                uint8_t quote);
bool adjustLastDigit(decimalSlice *d,
                     uint64_t currentDiff,
                     uint64_t targetDiff,
                     uint64_t maxDiff,
                     uint64_t ulpDecimal,
                     uint64_t ulpBinary);
bool adjustLastDigitFixed(decimalSlice *d,
                          uint64_t num,
                          uint64_t den,
                          unsigned int shift,
                          uint64_t ε);
moku::slice<uint8_t> appendEscapedRune(moku::slice<uint8_t> buf,
                                       int32_t r,
                                       uint8_t quote,
                                       bool ASCIIonly,
                                       bool graphicOnly);
moku::slice<uint8_t> appendQuotedRuneWith(moku::slice<uint8_t> buf,
                                          int32_t r,
                                          uint8_t quote,
                                          bool ASCIIonly,
                                          bool graphicOnly);
moku::slice<uint8_t> appendQuotedWith(moku::slice<uint8_t> buf,
                                      std::string s,
                                      uint8_t quote,
                                      bool ASCIIonly,
                                      bool graphicOnly);
std::tuple<float, moku::error> atof32(std::string s);
std::tuple<float, bool> atof32exact(uint64_t mantissa, int exp, bool neg);
std::tuple<double, moku::error> atof64(std::string s);
std::tuple<double, bool> atof64exact(uint64_t mantissa, int exp, bool neg);
moku::slice<uint8_t> bigFtoa(moku::slice<uint8_t> dst,
                             int prec,
                             uint8_t fmt,
                             bool neg,
                             uint64_t mant,
                             int exp,
                             floatInfo *flt);
int bsearch16(moku::slice<uint16_t> a, uint16_t x);
int bsearch32(moku::slice<uint32_t> a, uint32_t x);
bool contains(std::string s, uint8_t c);

struct decimal {
    std::vector<uint8_t> d;
    int nd{0};
    int dp{0};
    bool neg{false};
    bool trunc{false};
    bool set(std::string s);
    std::tuple<uint64_t, bool> floatBits(floatInfo *flt);
    std::string String();
    void Assign(uint64_t v);
    void Shift(int k);
    void Round(int nd);
    void RoundDown(int nd);
    void RoundUp(int nd);
    uint64_t RoundedInteger();
};
template <>
inline bool moku::is_nil<decimal>(const decimal &decimal)
{
    return moku::is_nil<std::vector<uint8_t>>(decimal.d) &&
           moku::is_nil<int>(decimal.nd) && moku::is_nil<int>(decimal.dp) &&
           moku::is_nil<bool>(decimal.neg) && moku::is_nil<bool>(decimal.trunc);
}
struct decimalSlice {
    moku::slice<uint8_t> d;
    int nd{0};
    int dp{0};
    bool neg{false};
};
template <>
inline bool moku::is_nil<decimalSlice>(const decimalSlice &decimalslice)
{
    return moku::is_nil<moku::slice<uint8_t>>(decimalslice.d) &&
           moku::is_nil<int>(decimalslice.nd) &&
           moku::is_nil<int>(decimalslice.dp) &&
           moku::is_nil<bool>(decimalslice.neg);
}
int digitZero(moku::slice<uint8_t> dst);
static constexpr std::string digits{"0123456789abcdefghijklmnopqrstuvwxyz"};
bool equalIgnoreCase(std::string s1, std::string s2);

struct extFloat {
    uint64_t mant{0};
    int exp{0};
    bool neg{false};
    std::tuple<uint64_t, bool> floatBits(floatInfo *flt);
    std::tuple<extFloat, extFloat> AssignComputeBounds(uint64_t mant,
                                                       int exp,
                                                       bool neg,
                                                       floatInfo *flt);
    unsigned int Normalize();
    void Multiply(extFloat g);
    bool AssignDecimal(
            uint64_t mantissa, int exp10, bool neg, bool trunc, floatInfo *flt);
    std::tuple<int, int> frexp10();
    bool FixedDecimal(decimalSlice *d, int n);
    bool ShortestDecimal(decimalSlice *d, extFloat *lower, extFloat *upper);
};
template <>
inline bool moku::is_nil<extFloat>(const extFloat &extfloat)
{
    return moku::is_nil<uint64_t>(extfloat.mant) &&
           moku::is_nil<int>(extfloat.exp) && moku::is_nil<bool>(extfloat.neg);
}
static constexpr bool fastSmalls{true};
static constexpr int firstPowerOfTen{-348};
static floatInfo float32info;
static moku::slice<float> float32pow10;
static floatInfo float64info;
static moku::slice<double> float64pow10;

struct floatInfo {
    unsigned int mantbits{0};
    unsigned int expbits{0};
    int bias{0};
};
template <>
inline bool moku::is_nil<floatInfo>(const floatInfo &floatinfo)
{
    return moku::is_nil<unsigned int>(floatinfo.mantbits) &&
           moku::is_nil<unsigned int>(floatinfo.expbits) &&
           moku::is_nil<int>(floatinfo.bias);
}
moku::slice<uint8_t> fmtB(moku::slice<uint8_t> dst,
                          bool neg,
                          uint64_t mant,
                          int exp,
                          floatInfo *flt);
moku::slice<uint8_t> fmtE(moku::slice<uint8_t> dst,
                          bool neg,
                          decimalSlice d,
                          int prec,
                          uint8_t fmt);
moku::slice<uint8_t> fmtF(moku::slice<uint8_t> dst,
                          bool neg,
                          decimalSlice d,
                          int prec);
static constexpr std::string fnParseFloat{"ParseFloat"};
std::tuple<moku::slice<uint8_t>, std::string> formatBits(
        moku::slice<uint8_t> dst, uint64_t u, int base, bool neg, bool append_);
moku::slice<uint8_t> formatDigits(moku::slice<uint8_t> dst,
                                  bool shortest,
                                  bool neg,
                                  decimalSlice digs,
                                  int prec,
                                  uint8_t fmt);
int frexp10Many(extFloat *a, extFloat *b, extFloat *c);
moku::slice<uint8_t> genericFtoa(moku::slice<uint8_t> dst,
                                 double val,
                                 uint8_t fmt,
                                 int prec,
                                 int bitSize);
static constexpr bool host32bit{false};
static constexpr int intSize{64};
static moku::slice<uint16_t> isGraphic;
bool isInGraphicList(int32_t r);
static moku::slice<uint16_t> isNotPrint16;
static moku::slice<uint16_t> isNotPrint32;
static moku::slice<uint16_t> isPrint16;
static moku::slice<uint32_t> isPrint32;

struct leftCheat {
    int delta{0};
    std::string cutoff{""};
};
template <>
inline bool moku::is_nil<leftCheat>(const leftCheat &leftcheat)
{
    return moku::is_nil<int>(leftcheat.delta) &&
           moku::is_nil<std::string>(leftcheat.cutoff);
}
void leftShift(decimal *a, unsigned int k);
static moku::slice<leftCheat> leftcheats;
static constexpr std::string lowerhex{"0123456789abcdef"};
int max(int a, int b);
static constexpr int maxShift{60};
static constexpr int maxUint64{18446744073709551615};
int min(int a, int b);
static constexpr int nSmalls{100};
static bool optimize;
static std::vector<extFloat> powersOfTen;
static moku::slice<int> powtab;
bool prefixIsLessThan(moku::slice<uint8_t> b, std::string s);
std::string quoteRuneWith(int32_t r,
                          uint8_t quote,
                          bool ASCIIonly,
                          bool graphicOnly);
std::string quoteWith(std::string s,
                      uint8_t quote,
                      bool ASCIIonly,
                      bool graphicOnly);
NumError *rangeError(std::string fn, std::string str);
std::tuple<uint64_t, int, bool, bool, bool> readFloat(std::string s);
void rightShift(decimal *a, unsigned int k);
void roundShortest(decimal *d, uint64_t mant, int exp, floatInfo *flt);
static std::vector<unsigned int> shifts;
bool shouldRoundUp(decimal *a, int nd);
std::string small(int i);
static std::vector<extFloat> smallPowersOfTen;
static constexpr std::string smallsString
{
    "00010203040506070809101112131415161718192021222324252627282930313233...};
            std::tuple<double, bool>
            special(std::string s);
    static constexpr int stepPowerOfTen{8};
    NumError *syntaxError(std::string fn, std::string str);
    void trim(decimal * a);
    static std::vector<uint64_t> uint64pow10;
    static constexpr int uintSize{64};
    std::tuple<int32_t, bool> unhex(uint8_t b);
} // namespace strconv
