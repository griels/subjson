#pragma once
#include "os.h"
#include "runtime.h"
#include "sort.h"
#include "time.h"
namespace debug {
void FreeOSMemory();

struct GCStats {
    Time LastGC;
    int64_t NumGC{0};
    Duration PauseTotal;
    moku::slice<Duration> Pause;
    moku::slice<Time> PauseEnd;
    moku::slice<Duration> PauseQuantiles;
};
template <>
inline bool moku::is_nil<GCStats>(const GCStats &gcstats)
{
    return moku::is_nil<Time>(gcstats.LastGC) &&
           moku::is_nil<int64_t>(gcstats.NumGC) &&
           moku::is_nil<Duration>(gcstats.PauseTotal) &&
           moku::is_nil<moku::slice<Duration>>(gcstats.Pause) &&
           moku::is_nil<moku::slice<Time>>(gcstats.PauseEnd) &&
           moku::is_nil<moku::slice<Duration>>(gcstats.PauseQuantiles);
}
void PrintStack();
void ReadGCStats(GCStats *stats);
int SetGCPercent(int percent);
int SetMaxStack(int bytes);
int SetMaxThreads(int threads);
bool SetPanicOnFault(bool enabled);
void SetTraceback(std::string level);
moku::slice<uint8_t> Stack();
void WriteHeapDump(uintptr_t fd);
void freeOSMemory();
void readGCStats(moku::slice<Duration> *);
int32_t setGCPercent(int32_t);
int setMaxStack(int);
int setMaxThreads(int);
bool setPanicOnFault(bool);
} // namespace debug
