#pragma once
#include "atomic.h"
#include "race.h"
#include "runtime.h"
#include "unsafe.h"
namespace sync {

struct Cond : public Locker {
    noCopy noCopy;
    Locker L;
    notifyList notify;
    copyChecker checker;
    void Wait();
    void Signal();
    void Broadcast();
};
template <>
inline bool moku::is_nil<Cond>(const Cond &cond)
{
    return moku::is_nil<noCopy>(cond.noCopy) && moku::is_nil<Locker>(cond.L) &&
           moku::is_nil<notifyList>(cond.notify) &&
           moku::is_nil<copyChecker>(cond.checker);
}
template <>
Cond *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct Locker {
    virtual void Unlock() = 0;
    virtual void Lock() = 0;
};
template <>
inline poolLocalInternal *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<poolLocalInternal>(iface);
}
template <>
inline copyChecker *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<copyChecker>(iface);
}
template <>
inline noCopy *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<noCopy>(iface);
}
template <>
inline notifyList *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<notifyList>(iface);
}
template <>
inline RWMutex *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<RWMutex>(iface);
}
template <>
inline Pool *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<Pool>(iface);
}
template <>
inline Mutex *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<Mutex>(iface);
}

struct Map : public Locker {
    Mutex mu;
    Value read;
    std::map<moku::interface, entry *> dirty;
    int misses{0};
    std::tuple<moku::interface, bool> Load(moku::interface key);
    void Store(moku::interface key, moku::interface value);
    std::tuple<moku::interface, bool> LoadOrStore(moku::interface key,
                                                  moku::interface value);
    void Delete(moku::interface key);
    void Range(std::function<bool(moku::interface, moku::interface)> f);
    void missLocked();
    void dirtyLocked();
};
template <>
inline bool moku::is_nil<Map>(const Map &map)
{
    return moku::is_nil<Mutex>(map.mu) && moku::is_nil<Value>(map.read) &&
           moku::is_nil<std::map<moku::interface, entry *>>(map.dirty) &&
           moku::is_nil<int>(map.misses);
}
template <>
Map *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct Mutex : public Locker {
    int32_t state{0};
    uint32_t sema{0};
    void Lock();
    void Unlock();
};
template <>
inline bool moku::is_nil<Mutex>(const Mutex &mutex)
{
    return moku::is_nil<int32_t>(mutex.state) &&
           moku::is_nil<uint32_t>(mutex.sema);
}
template <>
Mutex *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}
Cond *NewCond(Locker l);

struct Once : public Locker {
    Mutex m;
    uint32_t done{0};
    void Do(std::function<void()> f);
};
template <>
inline bool moku::is_nil<Once>(const Once &once)
{
    return moku::is_nil<Mutex>(once.m) && moku::is_nil<uint32_t>(once.done);
}
template <>
Once *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct Pool : public Locker {
    noCopy noCopy;
    void *local{std::nullptr};
    uintptr_t localSize{0};
    std::function<moku::interface()> New{std::nullptr};
    void Put(moku::interface x);
    moku::interface Get();
    moku::interface getSlow();
    poolLocal *pin();
    poolLocal *pinSlow();
};
template <>
inline bool moku::is_nil<Pool>(const Pool &pool)
{
    return moku::is_nil<noCopy>(pool.noCopy) &&
           moku::is_nil<void *>(pool.local) &&
           moku::is_nil<uintptr_t>(pool.localSize) &&
           moku::is_nil<std::function<moku::interface()>>(pool.New);
}
template <>
Pool *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct RWMutex : public Locker {
    Mutex w;
    uint32_t writerSem{0};
    uint32_t readerSem{0};
    int32_t readerCount{0};
    int32_t readerWait{0};
    void RLock();
    void RUnlock();
    void Lock();
    void Unlock();
    Locker RLocker();
};
template <>
inline bool moku::is_nil<RWMutex>(const RWMutex &rwmutex)
{
    return moku::is_nil<Mutex>(rwmutex.w) &&
           moku::is_nil<uint32_t>(rwmutex.writerSem) &&
           moku::is_nil<uint32_t>(rwmutex.readerSem) &&
           moku::is_nil<int32_t>(rwmutex.readerCount) &&
           moku::is_nil<int32_t>(rwmutex.readerWait);
}
template <>
RWMutex *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct WaitGroup : public Locker {
    noCopy noCopy;
    std::vector<uint8_t> state1;
    uint32_t sema{0};
    uint64_t *state();
    void Add(int delta);
    void Done();
    void Wait();
};
template <>
inline bool moku::is_nil<WaitGroup>(const WaitGroup &waitgroup)
{
    return moku::is_nil<noCopy>(waitgroup.noCopy) &&
           moku::is_nil<std::vector<uint8_t>>(waitgroup.state1) &&
           moku::is_nil<uint32_t>(waitgroup.sema);
}
template <>
WaitGroup *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}
static moku::slice<Pool *> allPools;
static Mutex allPoolsMu;

struct copyChecker : public moku::basic<uintptr_t>, public Locker {
    copyChecker() : moku::basic<uintptr_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<copyChecker>(const copyChecker &copychecker)
{
    return moku::is_nil<uintptr_t>(uintptr_t(copychecker));
}
template <>
copyChecker *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}
void check();

struct entry : public Locker {
    void *p{std::nullptr};
    std::tuple<moku::interface, bool> load();
    bool tryStore(moku::interface *i);
    bool unexpungeLocked();
    void storeLocked(moku::interface *i);
    std::tuple<moku::interface, bool, bool> tryLoadOrStore(moku::interface i);
    bool delete ();
    bool tryExpungeLocked();
};
template <>
inline bool moku::is_nil<entry>(const entry &entry)
{
    return moku::is_nil<void *>(entry.p);
}
template <>
entry *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}
static void *expunged;
uint32_t fastrand();
poolLocal *indexLocal(void *l, int i);
static constexpr int mutexLocked{1};
static constexpr int mutexStarving{4};
static constexpr int mutexWaiterShift{3};
static constexpr int mutexWoken{2};
entry *newEntry(moku::interface i);

struct noCopy : public Locker {
    void Lock();
};
template <>
inline bool moku::is_nil<noCopy>(const noCopy &nocopy)
{
    return;
}
template <>
noCopy *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct notifyList : public Locker {
    uint32_t wait{0};
    uint32_t notify{0};
    uintptr_t lock{0};
    void *head{std::nullptr};
    void *tail{std::nullptr};
};
template <>
inline bool moku::is_nil<notifyList>(const notifyList &notifylist)
{
    return moku::is_nil<uint32_t>(notifylist.wait) &&
           moku::is_nil<uint32_t>(notifylist.notify) &&
           moku::is_nil<uintptr_t>(notifylist.lock) &&
           moku::is_nil<void *>(notifylist.head) &&
           moku::is_nil<void *>(notifylist.tail);
}
template <>
notifyList *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}
void poolCleanup();

struct poolLocal : public Locker {
    poolLocalInternal poolLocalInternal;
    std::vector<uint8_t> pad;
};
template <>
inline bool moku::is_nil<poolLocal>(const poolLocal &poollocal)
{
    return moku::is_nil<poolLocalInternal>(poollocal.poolLocalInternal) &&
           moku::is_nil<std::vector<uint8_t>>(poollocal.pad);
}
template <>
poolLocal *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct poolLocalInternal : public Locker {
    moku::interface private;
    moku::slice<moku::interface> shared;
    Mutex Mutex;
};
template <>
inline bool moku::is_nil<poolLocalInternal>(
        const poolLocalInternal &poollocalinternal)
{
    return moku::is_nil<moku::interface>(poollocalinternal.private) &&
           moku::is_nil<moku::slice<moku::interface>>(
                   poollocalinternal.shared) &&
           moku::is_nil<Mutex>(poollocalinternal.Mutex);
}
template <>
poolLocalInternal *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}
void *poolRaceAddr(moku::interface x);
static std::vector<uint64_t> poolRaceHash;

struct readOnly : public Locker {
    std::map<moku::interface, entry *> m;
    bool amended{false};
};
template <>
inline bool moku::is_nil<readOnly>(const readOnly &readonly)
{
    return moku::is_nil<std::map<moku::interface, entry *>>(readonly.m) &&
           moku::is_nil<bool>(readonly.amended);
}
template <>
readOnly *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}

struct rlocker : public Locker {
    Mutex w;
    uint32_t writerSem{0};
    uint32_t readerSem{0};
    int32_t readerCount{0};
    int32_t readerWait{0};
    void Lock();
    void Unlock();
};
template <>
inline bool moku::is_nil<rlocker>(const rlocker &rlocker)
{
    return moku::is_nil<Mutex>(rlocker.w) &&
           moku::is_nil<uint32_t>(rlocker.writerSem) &&
           moku::is_nil<uint32_t>(rlocker.readerSem) &&
           moku::is_nil<int32_t>(rlocker.readerCount) &&
           moku::is_nil<int32_t>(rlocker.readerWait);
}
template <>
rlocker *moku::try_assert(const moku::interface &iface)
{
    if (Locker *locker = moku::type_registry::try_assert<Locker>(iface))
        return locker;
    return std::nullptr;
}
void runtime_Semacquire(uint32_t *s);
void runtime_SemacquireMutex(uint32_t *s, bool lifo);
void runtime_Semrelease(uint32_t *s, bool handoff);
bool runtime_canSpin(int i);
void runtime_doSpin();
int64_t runtime_nanotime();
uint32_t runtime_notifyListAdd(notifyList *l);
void runtime_notifyListCheck(uintptr_t size);
void runtime_notifyListNotifyAll(notifyList *l);
void runtime_notifyListNotifyOne(notifyList *l);
void runtime_notifyListWait(notifyList *l, uint32_t t);
int runtime_procPin();
void runtime_procUnpin();
void runtime_registerPoolCleanup(std::function<void()> cleanup);
static constexpr int rwmutexMaxReaders{1073741824};
static constexpr float starvationThresholdNs{1e+06};
void throw(std::string);
} // namespace sync
