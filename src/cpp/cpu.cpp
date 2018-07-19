void init();
int main()
{
    init();
    _main();
    return 0;
}
std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> cpuid(uint32_t eaxArg,
                                                         uint32_t ecxArg)
{
    uint32_t eax{0};
    uint32_t ebx{0};
    uint32_t ecx{0};
    uint32_t edx{0};
}
std::tuple<uint32_t, uint32_t> xgetbv()
{
    uint32_t eax{0};
    uint32_t edx{0};
}
void init()
{
    uint32_t ebx7{0};
    uint32_t ecx1{0};
    uint32_t edx1{0};
    uint32_t maxId{0};
    bool osSupportsAVX{false};

    std::tie(maxId, _, _, _) = cpuid(0, 0);
    if (maxId < 1) {
        return;
    }
    std::tie(_, _, ecx1, edx1) = cpuid(1, 0);
    X86.HasSSE2 = isSet(26, edx1);
    X86.HasSSE3 = isSet(0, ecx1);
    X86.HasPCLMULQDQ = isSet(1, ecx1);
    X86.HasSSSE3 = isSet(9, ecx1);
    X86.HasSSE41 = isSet(19, ecx1);
    X86.HasSSE42 = isSet(20, ecx1);
    X86.HasPOPCNT = isSet(23, ecx1);
    X86.HasAES = isSet(25, ecx1);
    X86.HasOSXSAVE = isSet(27, ecx1);
    osSupportsAVX = false;
    if (X86.HasOSXSAVE) {
        std::tie(eax, _) = xgetbv();
        osSupportsAVX = isSet(1, eax) && isSet(2, eax);
    }
    X86.HasAVX = isSet(28, ecx1) && osSupportsAVX;
    if (maxId < 7) {
        return;
    }
    std::tie(_, ebx7, _, _) = cpuid(7, 0);
    X86.HasBMI1 = isSet(3, ebx7);
    X86.HasAVX2 = isSet(5, ebx7) && osSupportsAVX;
    X86.HasBMI2 = isSet(8, ebx7);
    X86.HasERMS = isSet(9, ebx7);
}
bool isSet(unsigned int bitpos, uint32_t value)
{
    return value & (1 << bitpos) != 0;
}
void init();
int main()
{
    init();
    _main();
    return 0;
}
