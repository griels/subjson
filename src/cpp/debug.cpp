void ReadGCStats(GCStats *stats)
{
    constexpr int maxPause{256};
    int n{0};

    if (cap(stats->Pause) < 2 * maxPause + 3) {
        stats->Pause = make(moku::slice<time::Duration>, 2 * maxPause + 3);
    }
    readGCStats(&stats->Pause);
    n = len(stats->Pause) - 3;
    stats->LastGC = time::Unix(0, int64_t(stats->Pause[n]));
    stats->NumGC = int64_t(stats->Pause[n + 1]);
    stats->PauseTotal = stats->Pause[n + 2];
    n *= 2;
    stats->Pause = moku::slice_expr<moku::slice<Duration>>(stats->Pause, n);
    if (cap(stats->PauseEnd) < maxPause) {
        stats->PauseEnd = make(moku::slice<time::Time>, 0, maxPause);
    }
    stats->PauseEnd = moku::slice_expr<moku::slice<Time>>(stats->PauseEnd, 0);
    {
        Duration ns{};
        for (ns : moku::range_value<moku::slice<Duration>>(
                     moku::slice_expr<moku::slice<Duration>>(
                             stats->Pause, n, n + n))) {
            stats->PauseEnd =
                    append(stats->PauseEnd, time::Unix(0, int64_t(ns)));
        }
    }
    if (len(stats->PauseQuantiles) > 0) {
        if (n == 0) {
            {
                int i{0};
                for (i : moku::range_key<moku::slice<Duration>>(
                             stats->PauseQuantiles)) {
                    stats->PauseQuantiles[i] = 0;
                }
            }
        } else {
            sorted = moku::slice_expr<moku::slice<Duration>>(
                    stats->Pause, n, n + n);
            copy(sorted, stats->Pause);
            sort::Slice(moku::make_iface<moku::slice<Duration>>(sorted),
                        [=](int i, int j) mutable -> bool {
                            return sorted[i] < sorted[j];
                        });
            nq = len(stats->PauseQuantiles) - 1;
            {
                int i{0};
                for (i = 0; i < nq; i++) {
                    stats->PauseQuantiles[i] = sorted[len(sorted) * i / nq];
                }
            }
            stats->PauseQuantiles[nq] = sorted[len(sorted) - 1];
        }
    }
}
int SetGCPercent(int percent)
{
    return int(setGCPercent(int32_t(percent)));
}
void FreeOSMemory()
{
    freeOSMemory();
}
int SetMaxStack(int bytes)
{
    return setMaxStack(bytes);
}
int SetMaxThreads(int threads)
{
    return setMaxThreads(threads);
}
bool SetPanicOnFault(bool enabled)
{
    return setPanicOnFault(enabled);
}
void WriteHeapDump(uintptr_t fd)
{
}
void SetTraceback(std::string level)
{
}
int main()
{
    _main();
    return 0;
}
void PrintStack()
{
    os::Stderr->Write(Stack());
}
moku::slice<uint8_t> Stack()
{
    moku::slice<uint8_t> buf{};

    buf = make(moku::slice<byte>, 1024);
    while (true) {
        int n{0};

        n = runtime::Stack(buf, false);
        if (n < len(buf)) {
            return moku::slice_expr<moku::slice<uint8_t>>(buf, n);
        }
        buf = make(moku::slice<byte>, 2 * len(buf));
    }
}
int main()
{
    _main();
    return 0;
}
void readGCStats(moku::slice<Duration> *)
{
}
void freeOSMemory()
{
}
int setMaxStack(int)
{
}
int32_t setGCPercent(int32_t)
{
}
bool setPanicOnFault(bool)
{
}
int setMaxThreads(int)
{
}
int main()
{
    _main();
    return 0;
}
