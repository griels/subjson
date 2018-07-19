#pragma once
#include "math.h"
#include "sync.h"
namespace rand {
double ExpFloat64();
float Float32();
double Float64();
int Int();
int32_t Int31();
int32_t Int31n(int32_t n);
int64_t Int63();
int64_t Int63n(int64_t n);
int Intn(int n);
Rand *New(Source src);
Source NewSource(int64_t seed);
Zipf *NewZipf(Rand *r, double s, double v, uint64_t imax);
double NormFloat64();
moku::slice<int> Perm(int n);

struct Rand {
    Source src;
    Source64 s64;
    int64_t readVal{0};
    int8_t readPos{0};
    double ExpFloat64();
    double NormFloat64();
    void Seed(int64_t seed);
    int64_t Int63();
    uint32_t Uint32();
    uint64_t Uint64();
    int32_t Int31();
    int Int();
    int64_t Int63n(int64_t n);
    int32_t Int31n(int32_t n);
    int Intn(int n);
    double Float64();
    float Float32();
    moku::slice<int> Perm(int n);
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
};
template <>
inline bool moku::is_nil<Rand>(const Rand &rand)
{
    return moku::is_nil<Source>(rand.src) && moku::is_nil<Source64>(rand.s64) &&
           moku::is_nil<int64_t>(rand.readVal) &&
           moku::is_nil<int8_t>(rand.readPos);
}
std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
void Seed(int64_t seed);

struct Source {
    virtual void Seed(int64_t seed) = 0;
    virtual int64_t Int63() = 0;
};

struct Source64 {
    virtual uint64_t Uint64() = 0;
    virtual void Seed(int64_t seed) = 0;
    virtual int64_t Int63() = 0;
};
uint32_t Uint32();
uint64_t Uint64();

struct Zipf {
    Rand *r{std::nullptr};
    double imax{0};
    double v{0};
    double q{0};
    double s{0};
    double oneminusQ{0};
    double oneminusQinv{0};
    double hxm{0};
    double hx0minusHxm{0};
    double h(double x);
    double hinv(double x);
    uint64_t Uint64();
};
template <>
inline bool moku::is_nil<Zipf>(const Zipf &zipf)
{
    return moku::is_nil<Rand *>(zipf.r) && moku::is_nil<double>(zipf.imax) &&
           moku::is_nil<double>(zipf.v) && moku::is_nil<double>(zipf.q) &&
           moku::is_nil<double>(zipf.s) &&
           moku::is_nil<double>(zipf.oneminusQ) &&
           moku::is_nil<double>(zipf.oneminusQinv) &&
           moku::is_nil<double>(zipf.hxm) &&
           moku::is_nil<double>(zipf.hx0minusHxm);
}
static constexpr int _A{48271};
static constexpr int _LEN{607};
static constexpr int _M{2147483647};
static constexpr int _MASK{9223372036854775807};
static constexpr int _MAX{9223372036854775808};
static constexpr int _Q{44488};
static constexpr int _R{3399};
static constexpr int _TAP{273};
uint32_t absInt32(int32_t i);
static std::vector<float> fe;
static std::vector<float> fn;
static Rand *globalRand;
static std::vector<uint32_t> ke;
static std::vector<uint32_t> kn;

struct lockedSource {
    Mutex lk;
    Source64 src;
    int64_t Int63();
    uint64_t Uint64();
    void Seed(int64_t seed);
    void seedPos(int64_t seed, int8_t *readPos);
    std::tuple<int, moku::error> read(moku::slice<uint8_t> p,
                                      int64_t *readVal,
                                      int8_t *readPos);
};
template <>
inline bool moku::is_nil<lockedSource>(const lockedSource &lockedsource)
{
    return moku::is_nil<Mutex>(lockedsource.lk) &&
           moku::is_nil<Source64>(lockedsource.src);
}
static constexpr float re{7.69712};
std::tuple<int, moku::error> read(moku::slice<uint8_t> p,
                                  std::function<int64_t()> int63,
                                  int64_t *readVal,
                                  int8_t *readPos);
static constexpr float rn{3.44262};

struct rngSource {
    int tap{0};
    int feed{0};
    std::vector<int64_t> vec;
    void Seed(int64_t seed);
    int64_t Int63();
    uint64_t Uint64();
};
template <>
inline bool moku::is_nil<rngSource>(const rngSource &rngsource)
{
    return moku::is_nil<int>(rngsource.tap) &&
           moku::is_nil<int>(rngsource.feed) &&
           moku::is_nil<std::vector<int64_t>>(rngsource.vec);
}
static std::vector<int64_t> rng_cooked;
int32_t seedrand(int32_t x);
static std::vector<float> we;
static std::vector<float> wn;
} // namespace rand
