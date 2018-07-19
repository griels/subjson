#pragma once
namespace cpu {
constexpr int CacheLineSize{64};
x86 X86;
std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> cpuid(uint32_t eaxArg,
                                                         uint32_t ecxArg);
bool isSet(unsigned int bitpos, uint32_t value);

struct x86 {
    std::vector<uint8_t> _;
    bool HasAES{false};
    bool HasAVX{false};
    bool HasAVX2{false};
    bool HasBMI1{false};
    bool HasBMI2{false};
    bool HasERMS{false};
    bool HasOSXSAVE{false};
    bool HasPCLMULQDQ{false};
    bool HasPOPCNT{false};
    bool HasSSE2{false};
    bool HasSSE3{false};
    bool HasSSSE3{false};
    bool HasSSE41{false};
    bool HasSSE42{false};
    std::vector<uint8_t> _;
};
template <>
inline bool moku::is_nil<x86>(const x86 &x86)
{
    return moku::is_nil<std::vector<uint8_t>>(x86._) &&
           moku::is_nil<bool>(x86.HasAES) && moku::is_nil<bool>(x86.HasAVX) &&
           moku::is_nil<bool>(x86.HasAVX2) && moku::is_nil<bool>(x86.HasBMI1) &&
           moku::is_nil<bool>(x86.HasBMI2) && moku::is_nil<bool>(x86.HasERMS) &&
           moku::is_nil<bool>(x86.HasOSXSAVE) &&
           moku::is_nil<bool>(x86.HasPCLMULQDQ) &&
           moku::is_nil<bool>(x86.HasPOPCNT) &&
           moku::is_nil<bool>(x86.HasSSE2) && moku::is_nil<bool>(x86.HasSSE3) &&
           moku::is_nil<bool>(x86.HasSSSE3) &&
           moku::is_nil<bool>(x86.HasSSE41) &&
           moku::is_nil<bool>(x86.HasSSE42) &&
           moku::is_nil<std::vector<uint8_t>>(x86._);
}
std::tuple<uint32_t, uint32_t> xgetbv();
} // namespace cpu
