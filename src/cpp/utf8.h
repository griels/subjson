#pragma once
namespace utf8 {
std::tuple<int32_t, int> DecodeLastRune(moku::slice<uint8_t> p);
std::tuple<int32_t, int> DecodeLastRuneInString(std::string s);
std::tuple<int32_t, int> DecodeRune(moku::slice<uint8_t> p);
std::tuple<int32_t, int> DecodeRuneInString(std::string s);
int EncodeRune(moku::slice<uint8_t> p, int32_t r);
bool FullRune(moku::slice<uint8_t> p);
bool FullRuneInString(std::string s);
constexpr uint32_t MaxRune{1114111};
int RuneCount(moku::slice<uint8_t> p);
int RuneCountInString(std::string s);
constexpr uint32_t RuneError{65533};
int RuneLen(int32_t r);
constexpr int RuneSelf{128};
bool RuneStart(uint8_t b);
constexpr int UTFMax{4};
bool Valid(moku::slice<uint8_t> p);
bool ValidRune(int32_t r);
bool ValidString(std::string s);

struct acceptRange {
    uint8_t lo{0};
    uint8_t hi{0};
};
template <>
inline bool moku::is_nil<acceptRange>(const acceptRange &acceptrange)
{
    return moku::is_nil<uint8_t>(acceptrange.lo) &&
           moku::is_nil<uint8_t>(acceptrange.hi);
}
static std::vector<acceptRange> acceptRanges;
static constexpr int as{240};
static std::vector<uint8_t> first;
static constexpr int hicb{191};
static constexpr int locb{128};
static constexpr int mask2{31};
static constexpr int mask3{15};
static constexpr int mask4{7};
static constexpr int maskx{63};
static constexpr int rune1Max{127};
static constexpr int rune2Max{2047};
static constexpr int rune3Max{65535};
static constexpr int s1{2};
static constexpr int s2{19};
static constexpr int s3{3};
static constexpr int s4{35};
static constexpr int s5{52};
static constexpr int s6{4};
static constexpr int s7{68};
static constexpr int surrogateMax{57343};
static constexpr int surrogateMin{55296};
static constexpr int t1{0};
static constexpr int t2{192};
static constexpr int t3{224};
static constexpr int t4{240};
static constexpr int t5{248};
static constexpr int tx{128};
static constexpr int xx{241};
} // namespace utf8
