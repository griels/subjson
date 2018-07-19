#pragma once
#include "crypto.h"
#include "hash.h"
#include "runtime.h"
#include "unsafe.h"
namespace md5 {
constexpr int BlockSize{64};
Hash New();
constexpr int Size{16};
std::vector<uint8_t> Sum(moku::slice<uint8_t> data);
void block(digest *dig, moku::slice<uint8_t> p);
void blockGeneric(digest *dig, moku::slice<uint8_t> p);
static constexpr int chunk{64};

struct digest {
    std::vector<uint32_t> s;
    std::vector<uint8_t> x;
    int nx{0};
    uint64_t len{0};
    void Reset();
    int Size();
    int BlockSize();
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> p);
    moku::slice<uint8_t> Sum(moku::slice<uint8_t> in);
    std::vector<uint8_t> checkSum();
};
template <>
inline bool moku::is_nil<digest>(const digest &digest)
{
    return moku::is_nil<std::vector<uint32_t>>(digest.s) &&
           moku::is_nil<std::vector<uint8_t>>(digest.x) &&
           moku::is_nil<int>(digest.nx) && moku::is_nil<uint64_t>(digest.len);
}
static constexpr int init0{1732584193};
static constexpr int init1{4023233417};
static constexpr int init2{2562383102};
static constexpr int init3{271733878};
static bool littleEndian;
static constexpr bool x86{true};
} // namespace md5
