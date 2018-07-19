#pragma once
#include "cpu.h"
#include "crypto.h"
#include "hash.h"
namespace sha512 {
constexpr int BlockSize{128};
Hash New();
Hash New384();
Hash New512_224();
Hash New512_256();
constexpr int Size{64};
constexpr int Size224{28};
constexpr int Size256{32};
constexpr int Size384{48};
std::vector<uint8_t> Sum384(moku::slice<uint8_t> data);
std::vector<uint8_t> Sum512(moku::slice<uint8_t> data);
std::vector<uint8_t> Sum512_224(moku::slice<uint8_t> data);
std::vector<uint8_t> Sum512_256(moku::slice<uint8_t> data);
static moku::slice<uint64_t> _K;
void block(digest *dig, moku::slice<uint8_t> p);
void blockAMD64(digest *dig, moku::slice<uint8_t> p);
void blockAVX2(digest *dig, moku::slice<uint8_t> p);
void blockGeneric(digest *dig, moku::slice<uint8_t> p);
static constexpr int chunk{128};

struct digest {
    std::vector<uint64_t> h;
    std::vector<uint8_t> x;
    int nx{0};
    uint64_t len{0};
    Hash function;
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
    return moku::is_nil<std::vector<uint64_t>>(digest.h) &&
           moku::is_nil<std::vector<uint8_t>>(digest.x) &&
           moku::is_nil<int>(digest.nx) && moku::is_nil<uint64_t>(digest.len) &&
           moku::is_nil<Hash>(digest.function);
}
static constexpr int init0{7640891576956012808};
static constexpr int init0_224{10105294471447203234};
static constexpr int init0_256{2463787394917988140};
static constexpr int init0_384{14680500436340154072};
static constexpr int init1{13503953896175478587};
static constexpr int init1_224{8350123849800275158};
static constexpr int init1_256{11481187982095705282};
static constexpr int init1_384{7105036623409894663};
static constexpr int init2{4354685564936845355};
static constexpr int init2_224{2160240930085379202};
static constexpr int init2_256{2563595384472711505};
static constexpr int init2_384{10473403895298186519};
static constexpr int init3{11912009170470909681};
static constexpr int init3_224{7466358040605728719};
static constexpr int init3_256{10824532655140301501};
static constexpr int init3_384{1526699215303891257};
static constexpr int init4{5840696475078001361};
static constexpr int init4_224{1111592415079452072};
static constexpr int init4_256{10819967247969091555};
static constexpr int init4_384{7436329637833083697};
static constexpr int init5{11170449401992604703};
static constexpr int init5_224{8638871050018654530};
static constexpr int init5_256{13717434660681038226};
static constexpr int init5_384{10282925794625328401};
static constexpr int init6{2270897969802886507};
static constexpr int init6_224{4583966954114332360};
static constexpr int init6_256{3098927326965381290};
static constexpr int init6_384{15784041429090275239};
static constexpr int init7{6620516959819538809};
static constexpr int init7_224{1230299281376055969};
static constexpr int init7_256{1060366662362279074};
static constexpr int init7_384{5167115440072839076};
static bool useAVX2;
} // namespace sha512
