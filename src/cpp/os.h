#pragma once
#include "atomic.h"
#include "errors.h"
#include "io.h"
#include "poll.h"
#include "runtime.h"
#include "sync.h"
#include "syscall.h"
#include "time.h"
namespace os {
moku::slice<std::string> Args;
moku::error Chdir(std::string dir);
moku::error Chmod(std::string name, FileMode mode);
moku::error Chown(std::string name, int uid, int gid);
moku::error Chtimes(std::string name, Time atime, Time mtime);
void Clearenv();
std::tuple<File *, moku::error> Create(std::string name);
constexpr std::string DevNull{"/dev/null"};
moku::slice<std::string> Environ();
moku::error ErrClosed;
moku::error ErrExist;
moku::error ErrInvalid;
moku::error ErrNotExist;
moku::error ErrPermission;
std::tuple<std::string, moku::error> Executable();
void Exit(int code);
std::string Expand(std::string s,
                   std::function<std::string(std::string)> mapping);
std::string ExpandEnv(std::string s);

struct File : public FileInfo, public Signal {
    file *file{std::nullptr};
    std::tuple<moku::slice<FileInfo>, moku::error> Readdir(int n);
    std::tuple<moku::slice<std::string>, moku::error> Readdirnames(int n);
    std::tuple<moku::slice<FileInfo>, moku::error> readdir(int n);
    std::tuple<moku::slice<std::string>, moku::error> readdirnames(int n);
    std::string Name();
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> b);
    std::tuple<int, moku::error> ReadAt(moku::slice<uint8_t> b, int64_t off);
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> b);
    std::tuple<int, moku::error> WriteAt(moku::slice<uint8_t> b, int64_t off);
    std::tuple<int64_t, moku::error> Seek(int64_t offset, int whence);
    std::tuple<int, moku::error> WriteString(std::string s);
    moku::error wrapErr(std::string op, moku::error err);
    moku::error Chmod(FileMode mode);
    moku::error chmod(FileMode mode);
    moku::error Chown(int uid, int gid);
    moku::error Truncate(int64_t size);
    moku::error Sync();
    moku::error Chdir();
    moku::error checkValid(std::string op);
    uintptr_t Fd();
    moku::error Close();
    std::tuple<int, moku::error> read(moku::slice<uint8_t> b);
    std::tuple<int, moku::error> pread(moku::slice<uint8_t> b, int64_t off);
    std::tuple<int, moku::error> write(moku::slice<uint8_t> b);
    std::tuple<int, moku::error> pwrite(moku::slice<uint8_t> b, int64_t off);
    std::tuple<int64_t, moku::error> seek(int64_t offset, int whence);
    std::tuple<FileInfo, moku::error> Stat();
};
template <>
inline bool moku::is_nil<File>(const File &file)
{
    return moku::is_nil<file *>(file.file);
}
template <>
File *moku::try_assert(const moku::interface &iface)
{
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    return std::nullptr;
}

struct FileInfo {
    virtual moku::interface Sys() = 0;
    virtual int64_t Size() = 0;
    virtual std::string Name() = 0;
    virtual FileMode Mode() = 0;
    virtual Time ModTime() = 0;
    virtual bool IsDir() = 0;
};
template <>
inline file *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<file>(iface);
}
template <>
inline fileStat *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<fileStat>(iface);
}
template <>
inline File *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<File>(iface);
}
template <>
inline PathError *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<PathError>(iface);
}

struct FileMode : public moku::basic<uint32_t>, public Signal, public FileInfo {
    FileMode() : moku::basic<uint32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<FileMode>(const FileMode &filemode)
{
    return moku::is_nil<uint32_t>(uint32_t(filemode));
}
template <>
FileMode *moku::try_assert(const moku::interface &iface)
{
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    return std::nullptr;
}
inline std::string String()
{
    FileMode filemode = *this;
    return filemode._StringByValue();
}
std::string _StringByValue();
inline bool IsDir()
{
    FileMode filemode = *this;
    return filemode._IsDirByValue();
}
bool _IsDirByValue();
inline bool IsRegular()
{
    FileMode filemode = *this;
    return filemode._IsRegularByValue();
}
bool _IsRegularByValue();
inline FileMode Perm()
{
    FileMode filemode = *this;
    return filemode._PermByValue();
}
FileMode _PermByValue();
std::tuple<Process *, moku::error> FindProcess(int pid);
int Getegid();
std::string Getenv(std::string key);
int Geteuid();
int Getgid();
std::tuple<moku::slice<int>, moku::error> Getgroups();
int Getpagesize();
int Getpid();
int Getppid();
int Getuid();
std::tuple<std::string, moku::error> Getwd();
std::tuple<std::string, moku::error> Hostname();
Signal Interrupt;
bool IsExist(moku::error err);
bool IsNotExist(moku::error err);
bool IsPathSeparator(uint8_t c);
bool IsPermission(moku::error err);
Signal Kill;
moku::error Lchown(std::string name, int uid, int gid);
moku::error Link(std::string oldname, std::string newname);

struct LinkError : public Signal, public FileInfo {
    std::string Op{""};
    std::string Old{""};
    std::string New{""};
    moku::error Err;
    std::string Error();
};
template <>
inline bool moku::is_nil<LinkError>(const LinkError &linkerror)
{
    return moku::is_nil<std::string>(linkerror.Op) &&
           moku::is_nil<std::string>(linkerror.Old) &&
           moku::is_nil<std::string>(linkerror.New) &&
           moku::is_nil<moku::error>(linkerror.Err);
}
template <>
LinkError *moku::try_assert(const moku::interface &iface)
{
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    return std::nullptr;
}
std::tuple<std::string, bool> LookupEnv(std::string key);
std::tuple<FileInfo, moku::error> Lstat(std::string name);
moku::error Mkdir(std::string name, FileMode perm);
moku::error MkdirAll(std::string path, FileMode perm);
constexpr FileMode ModeAppend{1073741824};
constexpr FileMode ModeCharDevice{2097152};
constexpr FileMode ModeDevice{67108864};
constexpr FileMode ModeDir{2147483648};
constexpr FileMode ModeExclusive{536870912};
constexpr FileMode ModeNamedPipe{33554432};
constexpr FileMode ModePerm{511};
constexpr FileMode ModeSetgid{4194304};
constexpr FileMode ModeSetuid{8388608};
constexpr FileMode ModeSocket{16777216};
constexpr FileMode ModeSticky{1048576};
constexpr FileMode ModeSymlink{134217728};
constexpr FileMode ModeTemporary{268435456};
constexpr FileMode ModeType{2399141888};
File *NewFile(uintptr_t fd, std::string name);
moku::error NewSyscallError(std::string syscall, moku::error err);
constexpr int O_APPEND{8};
constexpr int O_CREATE{512};
constexpr int O_EXCL{2048};
constexpr int O_RDONLY{0};
constexpr int O_RDWR{2};
constexpr int O_SYNC{128};
constexpr int O_TRUNC{1024};
constexpr int O_WRONLY{1};
std::tuple<File *, moku::error> Open(std::string name);
std::tuple<File *, moku::error> OpenFile(std::string name,
                                         int flag,
                                         FileMode perm);

struct PathError : public Signal, public FileInfo {
    std::string Op{""};
    std::string Path{""};
    moku::error Err;
    std::string Error();
};
template <>
inline bool moku::is_nil<PathError>(const PathError &patherror)
{
    return moku::is_nil<std::string>(patherror.Op) &&
           moku::is_nil<std::string>(patherror.Path) &&
           moku::is_nil<moku::error>(patherror.Err);
}
template <>
PathError *moku::try_assert(const moku::interface &iface)
{
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    return std::nullptr;
}
constexpr uint32_t PathListSeparator{58};
constexpr uint32_t PathSeparator{47};
std::tuple<File *, File *, moku::error> Pipe();

struct ProcAttr : public Signal, public FileInfo {
    std::string Dir{""};
    moku::slice<std::string> Env;
    moku::slice<File *> Files;
    SysProcAttr *Sys{std::nullptr};
};
template <>
inline bool moku::is_nil<ProcAttr>(const ProcAttr &procattr)
{
    return moku::is_nil<std::string>(procattr.Dir) &&
           moku::is_nil<moku::slice<std::string>>(procattr.Env) &&
           moku::is_nil<moku::slice<File *>>(procattr.Files) &&
           moku::is_nil<SysProcAttr *>(procattr.Sys);
}
template <>
ProcAttr *moku::try_assert(const moku::interface &iface)
{
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    return std::nullptr;
}

struct Process : public Signal, public FileInfo {
    int Pid{0};
    uintptr_t handle{0};
    uint32_t isdone{0};
    RWMutex sigMu;
    void setDone();
    bool done();
    moku::error Release();
    moku::error Kill();
    std::tuple<ProcessState *, moku::error> Wait();
    moku::error Signal(Signal sig);
    moku::error kill();
    std::tuple<ProcessState *, moku::error> wait();
    moku::error signal(Signal sig);
    moku::error release();
    std::tuple<bool, moku::error> blockUntilWaitable();
};
template <>
inline bool moku::is_nil<Process>(const Process &process)
{
    return moku::is_nil<int>(process.Pid) &&
           moku::is_nil<uintptr_t>(process.handle) &&
           moku::is_nil<uint32_t>(process.isdone) &&
           moku::is_nil<RWMutex>(process.sigMu);
}
template <>
Process *moku::try_assert(const moku::interface &iface)
{
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    return std::nullptr;
}

struct ProcessState : public FileInfo, public Signal {
    int pid{0};
    WaitStatus status;
    Rusage *rusage{std::nullptr};
    Duration UserTime();
    Duration SystemTime();
    bool Exited();
    bool Success();
    moku::interface Sys();
    moku::interface SysUsage();
    int Pid();
    bool exited();
    bool success();
    moku::interface sys();
    moku::interface sysUsage();
    std::string String();
    Duration userTime();
    Duration systemTime();
};
template <>
inline bool moku::is_nil<ProcessState>(const ProcessState &processstate)
{
    return moku::is_nil<int>(processstate.pid) &&
           moku::is_nil<WaitStatus>(processstate.status) &&
           moku::is_nil<Rusage *>(processstate.rusage);
}
template <>
ProcessState *moku::try_assert(const moku::interface &iface)
{
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    return std::nullptr;
}
std::tuple<std::string, moku::error> Readlink(std::string name);
moku::error Remove(std::string name);
moku::error RemoveAll(std::string path);
moku::error Rename(std::string oldpath, std::string newpath);
constexpr int SEEK_CUR{1};
constexpr int SEEK_END{2};
constexpr int SEEK_SET{0};
bool SameFile(FileInfo fi1, FileInfo fi2);
moku::error Setenv(std::string key, std::string value);

struct Signal {
    virtual std::string String() = 0;
    virtual void Signal() = 0;
};
template <>
inline LinkError *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<LinkError>(iface);
}
template <>
inline Process *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<Process>(iface);
}
std::tuple<Process *, moku::error> StartProcess(std::string name,
                                                moku::slice<std::string> argv,
                                                ProcAttr *attr);
std::tuple<FileInfo, moku::error> Stat(std::string name);
File *Stderr;
File *Stdin;
File *Stdout;
moku::error Symlink(std::string oldname, std::string newname);

struct SyscallError : public FileInfo, public Signal {
    std::string Syscall{""};
    moku::error Err;
    std::string Error();
};
template <>
inline bool moku::is_nil<SyscallError>(const SyscallError &syscallerror)
{
    return moku::is_nil<std::string>(syscallerror.Syscall) &&
           moku::is_nil<moku::error>(syscallerror.Err);
}
template <>
SyscallError *moku::try_assert(const moku::interface &iface)
{
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    return std::nullptr;
}
std::string TempDir();
moku::error Truncate(std::string name, int64_t size);
moku::error Unsetenv(std::string key);
Time atime(FileInfo fi);
std::string basename(std::string name);
static constexpr int blockSize{4096};
moku::error chmod(std::string name, FileMode mode);

struct dirInfo : public Signal, public FileInfo {
    moku::slice<uint8_t> buf;
    int nbuf{0};
    int bufp{0};
};
template <>
inline bool moku::is_nil<dirInfo>(const dirInfo &dirinfo)
{
    return moku::is_nil<moku::slice<uint8_t>>(dirinfo.buf) &&
           moku::is_nil<int>(dirinfo.nbuf) && moku::is_nil<int>(dirinfo.bufp);
}
template <>
dirInfo *moku::try_assert(const moku::interface &iface)
{
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    return std::nullptr;
}
void epipecheck(File *file, moku::error e);
static moku::error errFinished;
std::tuple<std::string, moku::error> executable();
static std::string executablePath;

struct file : public Signal, public FileInfo {
    FD pfd;
    std::string name{""};
    dirInfo *dirinfo{std::nullptr};
    bool nonblock{false};
    moku::error close();
};
template <>
inline bool moku::is_nil<file>(const file &file)
{
    return moku::is_nil<FD>(file.pfd) && moku::is_nil<std::string>(file.name) &&
           moku::is_nil<dirInfo *>(file.dirinfo) &&
           moku::is_nil<bool>(file.nonblock);
}
template <>
file *moku::try_assert(const moku::interface &iface)
{
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    return std::nullptr;
}

struct fileStat : public FileInfo, public Signal {
    std::string name{""};
    int64_t size{0};
    FileMode mode;
    Time modTime;
    Stat_t sys;
    std::string Name();
    bool IsDir();
    int64_t Size();
    FileMode Mode();
    Time ModTime();
    moku::interface Sys();
};
template <>
inline bool moku::is_nil<fileStat>(const fileStat &filestat)
{
    return moku::is_nil<std::string>(filestat.name) &&
           moku::is_nil<int64_t>(filestat.size) &&
           moku::is_nil<FileMode>(filestat.mode) &&
           moku::is_nil<Time>(filestat.modTime) &&
           moku::is_nil<Stat_t>(filestat.sys);
}
template <>
fileStat *moku::try_assert(const moku::interface &iface)
{
    if (FileInfo *fileinfo = moku::type_registry::try_assert<FileInfo>(iface))
        return fileinfo;
    if (Signal *signal = moku::type_registry::try_assert<Signal>(iface))
        return signal;
    return std::nullptr;
}
void fillFileStatFromSys(fileStat *fs, std::string name);
std::tuple<Process *, moku::error> findProcess(int pid);
std::tuple<int, moku::error> fixCount(int n, moku::error err);
std::string fixLongPath(std::string path);
std::tuple<std::string, int> getShellName(std::string s);
static struct {
    Mutex Mutex;
    std::string dir{""};
} getwdCache;
std::tuple<std::string, moku::error> hostname();
static std::string initCwd;
static moku::error initCwdErr;
bool isAlphaNum(uint8_t c);
bool isExist(moku::error err);
bool isNotExist(moku::error err);
bool isPermission(moku::error err);
bool isShellSpecialVar(uint8_t c);
std::string itoa(int val);
static std::function<FileInfo, moku::error(std::string)> lstat;
File *newFile(uintptr_t fd, std::string name, bool pollable);
Process *newProcess(int pid, uintptr_t handle);
moku::error rename(std::string oldname, std::string newname);
moku::slice<std::string> runtime_args();
void runtime_beforeExit();
bool sameFile(fileStat *fs1, fileStat *fs2);
void sigpipe();
std::tuple<Process *, moku::error> startProcess(std::string name,
                                                moku::slice<std::string> argv,
                                                ProcAttr *attr);
static constexpr bool supportsCloseOnExec{true};
static constexpr bool supportsCreateWithStickyBit{false};
uint32_t syscallMode(FileMode i);
std::string tempDir();
Time timespecToTime(Timespec ts);
std::string uitoa(unsigned int val);
moku::error underlyingError(moku::error err);
static std::function<bool(moku::error)> useSyscallwd;
bool useSyscallwdDarwin(moku::error err);
moku::error wrapSyscallError(std::string name, moku::error err);
} // namespace os
