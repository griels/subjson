#pragma once
#include "atomic.h"
#include "errors.h"
#include "io.h"
#include "sync.h"
#include "syscall.h"
#include "time.h"
#include "unsafe.h"
namespace poll {
std::function<int, Sockaddr, moku::error(int)> AcceptFunc;
std::function<moku::error(int)> CloseFunc;
moku::error ErrFileClosing;
moku::error ErrNetClosing;
moku::error ErrTimeout;

struct FD {
    fdMutex fdmu;
    int Sysfd{0};
    pollDesc pd;
    moku::slice<Iovec> *iovecs{std::nullptr};
    bool IsStream{false};
    bool ZeroReadIsEOF{false};
    bool isFile{false};
    moku::error incref();
    moku::error decref();
    moku::error readLock();
    void readUnlock();
    moku::error writeLock();
    void writeUnlock();
    moku::error SetDeadline(Time t);
    moku::error SetReadDeadline(Time t);
    moku::error SetWriteDeadline(Time t);
    moku::error eofError(int n, moku::error err);
    moku::error Fchmod(uint32_t mode);
    moku::error Fchown(int uid, int gid);
    moku::error Ftruncate(int64_t size);
    moku::error Fsync();
    moku::error Init(std::string net, bool pollable);
    moku::error destroy();
    moku::error Close();
    moku::error Shutdown(int how);
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
    std::tuple<int, moku::error> Pread(moku::slice<uint8_t> p, int64_t off);
    std::tuple<int, Sockaddr, moku::error> ReadFrom(moku::slice<uint8_t> p);
    std::tuple<int, int, int, Sockaddr, moku::error> ReadMsg(
            moku::slice<uint8_t> p, moku::slice<uint8_t> oob);
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> p);
    std::tuple<int, moku::error> Pwrite(moku::slice<uint8_t> p, int64_t off);
    std::tuple<int, moku::error> WriteTo(moku::slice<uint8_t> p, Sockaddr sa);
    std::tuple<int, int, moku::error> WriteMsg(moku::slice<uint8_t> p,
                                               moku::slice<uint8_t> oob,
                                               Sockaddr sa);
    std::tuple<int, Sockaddr, std::string, moku::error> Accept();
    std::tuple<int64_t, moku::error> Seek(int64_t offset, int whence);
    std::tuple<int, moku::error> ReadDirent(moku::slice<uint8_t> buf);
    moku::error Fchdir();
    moku::error Fstat(Stat_t *s);
    moku::error WaitWrite();
    moku::error RawControl(std::function<void(uintptr_t)> f);
    moku::error RawRead(std::function<bool(uintptr_t)> f);
    moku::error RawWrite(std::function<bool(uintptr_t)> f);
    moku::error SetsockoptInt(int level, int name, int arg);
    moku::error SetsockoptInet4Addr(int level,
                                    int name,
                                    std::vector<uint8_t> arg);
    moku::error SetsockoptLinger(int level, int name, Linger *l);
    moku::error SetsockoptByte(int level, int name, uint8_t arg);
    moku::error SetsockoptIPMreq(int level, int name, IPMreq *mreq);
    moku::error SetsockoptIPv6Mreq(int level, int name, IPv6Mreq *mreq);
    std::tuple<int64_t, moku::error> Writev(
            moku::slice<moku::slice<uint8_t>> *v);
};
template <>
inline bool moku::is_nil<FD>(const FD &fd)
{
    return moku::is_nil<fdMutex>(fd.fdmu) && moku::is_nil<int>(fd.Sysfd) &&
           moku::is_nil<pollDesc>(fd.pd) &&
           moku::is_nil<moku::slice<Iovec> *>(fd.iovecs) &&
           moku::is_nil<bool>(fd.IsStream) &&
           moku::is_nil<bool>(fd.ZeroReadIsEOF) &&
           moku::is_nil<bool>(fd.isFile);
}
uintptr_t PollDescriptor();
std::function<void(int)> TestHookDidWritev;

struct TimeoutError {
    std::string Error();
    bool Timeout();
    bool Temporary();
};
template <>
inline bool moku::is_nil<TimeoutError>(const TimeoutError &timeouterror)
{
    return;
}
std::tuple<int, Sockaddr, std::string, moku::error> accept(int s);
void consume(moku::slice<moku::slice<uint8_t>> *v, int64_t n);
moku::error convertErr(int res, bool isFile);
moku::error errClosing(bool isFile);

struct fdMutex {
    uint64_t state{0};
    uint32_t rsema{0};
    uint32_t wsema{0};
    bool incref();
    bool increfAndClose();
    bool decref();
    bool rwlock(bool read);
    bool rwunlock(bool read);
};
template <>
inline bool moku::is_nil<fdMutex>(const fdMutex &fdmutex)
{
    return moku::is_nil<uint64_t>(fdmutex.state) &&
           moku::is_nil<uint32_t>(fdmutex.rsema) &&
           moku::is_nil<uint32_t>(fdmutex.wsema);
}
static constexpr int maxRW{1073741824};
static constexpr int mutexClosed{1};
static constexpr int mutexRLock{2};
static constexpr int mutexRMask{8796084633600};
static constexpr int mutexRWait{8388608};
static constexpr int mutexRef{8};
static constexpr int mutexRefMask{8388600};
static constexpr int mutexWLock{4};
static constexpr int mutexWMask{9223363240761753600};
static constexpr int mutexWWait{8796093022208};

struct pollDesc {
    uintptr_t runtimeCtx{0};
    moku::error init(FD *fd);
    void close();
    void evict();
    moku::error prepare(int mode, bool isFile);
    moku::error prepareRead(bool isFile);
    moku::error prepareWrite(bool isFile);
    moku::error wait(int mode, bool isFile);
    moku::error waitRead(bool isFile);
    moku::error waitWrite(bool isFile);
    void waitCanceled(int mode);
    bool pollable();
};
template <>
inline bool moku::is_nil<pollDesc>(const pollDesc &polldesc)
{
    return moku::is_nil<uintptr_t>(polldesc.runtimeCtx);
}
int64_t runtimeNano();
void runtime_Semacquire(uint32_t *sema);
void runtime_Semrelease(uint32_t *sema);
void runtime_pollClose(uintptr_t ctx);
std::tuple<uintptr_t, int> runtime_pollOpen(uintptr_t fd);
int runtime_pollReset(uintptr_t ctx, int mode);
uintptr_t runtime_pollServerDescriptor();
void runtime_pollServerInit();
void runtime_pollSetDeadline(uintptr_t ctx, int64_t d, int mode);
void runtime_pollUnblock(uintptr_t ctx);
int runtime_pollWait(uintptr_t ctx, int mode);
int runtime_pollWaitCanceled(uintptr_t ctx, int mode);
static Once serverInit;
moku::error setDeadlineImpl(FD *fd, Time t, int mode);
} // namespace poll
