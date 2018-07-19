std::tuple<moku::slice<FileInfo>, moku::error> File::Readdir(int n)
{
    if (moku::is_nil(this)) {
        return {nil, ErrInvalid};
    }
    return this->readdir(n);
}
std::tuple<moku::slice<std::string>, moku::error> File::Readdirnames(int n)
{
    moku::error err{};
    moku::slice<std::string> names{};

    if (moku::is_nil(this)) {
        return {nil, ErrInvalid};
    }
    return this->readdirnames(n);
}
void init();
int main()
{
    init();
    ErrInvalid = errors::New("invalid argument");
    ErrPermission = errors::New("permission denied");
    ErrExist = errors::New("file already exists");
    ErrNotExist = errors::New("file does not exist");
    ErrClosed = errors::New("file already closed");
    Interrupt = syscall::SIGINT;
    Kill = syscall::SIGKILL;
    errFinished = errors::New("os: process already finished");
    Stdin = NewFile(uintptr_t(syscall::Stdin), "/dev/stdin");
    Stdout = NewFile(uintptr_t(syscall::Stdout), "/dev/stdout");
    Stderr = NewFile(uintptr_t(syscall::Stderr), "/dev/stderr");
    useSyscallwd = [=](moku::error) mutable -> bool { return true; };
    lstat = Lstat;
    std::tie(initCwd, initCwdErr) = Getwd();
    _main();
    return 0;
}
std::tuple<moku::slice<FileInfo>, moku::error> File::readdir(int n)
{
    std::string dirname{""};
    moku::error err{};
    moku::slice<FileInfo> fi{};
    moku::slice<std::string> names{};

    dirname = this->name;
    if (dirname == "") {
        dirname = ".";
    }
    std::tie(names, err) = this->Readdirnames(n);
    fi = make(moku::slice<FileInfo>, 0, len(names));
    {
        std::string filename{""};
        for (filename : moku::range_value<moku::slice<std::string>>(names)) {
            std::tie(fip, lerr) = lstat(dirname + "/" + filename);
            if (IsNotExist(lerr)) {
                continue;
            }
            if (!moku::is_nil(lerr)) {
                return {fi, lerr};
            }
            fi = append(fi, fip);
        }
    }
    if (len(fi) == 0 && moku::is_nil(err) && n > 0) {
        err = io::EOF;
    }
    return {fi, err};
}
std::tuple<moku::slice<std::string>, moku::error> File::readdirnames(int n)
{
    dirInfo *d{std::nullptr};
    moku::error err{};
    moku::slice<std::string> names{};
    int size{0};

    if (moku::is_nil(this->dirinfo)) {
        this->dirinfo = new (dirInfo);
        this->dirinfo->buf = make(moku::slice<byte>, blockSize);
    }
    d = this->dirinfo;
    size = n;
    if (size <= 0) {
        size = 100;
        n = -1;
    }
    names = make(moku::slice<std::string>, 0, size);
    while (n != 0) {
        int nb{0};
        int nc{0};

        if (d->bufp >= d->nbuf) {
            d->bufp = 0;
            std::tie(d->nbuf, errno) = this->pfd.ReadDirent(d->buf);
            runtime::KeepAlive(moku::make_iface<File *>(f));
            if (!moku::is_nil(errno)) {
                return {names, wrapSyscallError("readdirent", errno)};
            }
            if (d->nbuf <= 0) {
                break;
            }
        }
        std::tie(nb, nc, names) =
                syscall::ParseDirent(moku::slice_expr<moku::slice<uint8_t>>(
                                             d->buf, d->bufp, d->nbuf),
                                     n,
                                     names);
        d->bufp += nb;
        n -= nc;
    }
    if (n >= 0 && len(names) == 0) {
        return {names, io::EOF};
    }
    return {names, nil};
}
void init();
int main()
{
    init();
    ErrInvalid = errors::New("invalid argument");
    ErrPermission = errors::New("permission denied");
    ErrExist = errors::New("file already exists");
    ErrNotExist = errors::New("file does not exist");
    ErrClosed = errors::New("file already closed");
    Interrupt = syscall::SIGINT;
    Kill = syscall::SIGKILL;
    errFinished = errors::New("os: process already finished");
    Stdin = NewFile(uintptr_t(syscall::Stdin), "/dev/stdin");
    Stdout = NewFile(uintptr_t(syscall::Stdout), "/dev/stdout");
    Stderr = NewFile(uintptr_t(syscall::Stderr), "/dev/stderr");
    useSyscallwd = [=](moku::error) mutable -> bool { return true; };
    lstat = Lstat;
    std::tie(initCwd, initCwdErr) = Getwd();
    _main();
    return 0;
}
std::string Expand(std::string s,
                   std::function<std::string(std::string)> mapping)
{
    moku::slice<uint8_t> buf{};
    int i{0};

    buf = make(moku::slice<byte>, 0, 2 * len(s));
    i = 0;
    {
        int j{0};
        for (j = 0; j < len(s); j++) {
            if (s[j] == '$' && j + 1 < len(s)) {
                buf = append(buf, moku::slice_expr<std::string>(s, i, j));
                std::tie(name, w) =
                        getShellName(moku::slice_expr<std::string>(s, j + 1));
                buf = append(buf, mapping(name));
                j += w;
                i = j + 1;
            }
        }
    }
    return std::string(buf) + moku::slice_expr<std::string>(s, i);
}
std::string ExpandEnv(std::string s)
{
    return Expand(s, Getenv);
}
bool isShellSpecialVar(uint8_t c)
{
    if ((c == '*') || (c == '#') || (c == '$') || (c == '@') || (c == '!') ||
        (c == '?') || (c == '-') || (c == '0') || (c == '1') || (c == '2') ||
        (c == '3') || (c == '4') || (c == '5') || (c == '6') || (c == '7') ||
        (c == '8') || (c == '9')) {
    _ident_0_:
        return true;
    }
    return false;
}
bool isAlphaNum(uint8_t c)
{
    return c == '_' || '0' <= c && c <= '9' || 'a' <= c && c <= 'z' ||
           'A' <= c && c <= 'Z';
}
std::tuple<std::string, int> getShellName(std::string s)
{
    int i{0};

    if ((s[0] == '{')) {
    _ident_1_:
        if (len(s) > 2 && isShellSpecialVar(s[1]) && s[2] == '}') {
            return {moku::slice_expr<std::string>(s, 1, 2), 3};
        }
        {
            int i{0};
            for (i = 1; i < len(s); i++) {
                if (s[i] == '}') {
                    return {moku::slice_expr<std::string>(s, 1, i), i + 1};
                }
            }
        }
        return {"", 1};
    } else if ((isShellSpecialVar(s[0]))) {
    _ident_2_:
        return {moku::slice_expr<std::string>(s, 0, 1), 1};
    }
    for (i = 0; i < len(s) && isAlphaNum(s[i]); i++) {
    }
    return {moku::slice_expr<std::string>(s, i), i};
}
std::string Getenv(std::string key)
{
    std::string v{""};

    std::tie(v, _) = syscall::Getenv(key);
    return v;
}
std::tuple<std::string, bool> LookupEnv(std::string key)
{
    return syscall::Getenv(key);
}
moku::error Setenv(std::string key, std::string value)
{
    moku::error err{};

    err = syscall::Setenv(key, value);
    if (!moku::is_nil(err)) {
        return NewSyscallError("setenv", err);
    }
    return nil;
}
moku::error Unsetenv(std::string key)
{
    return syscall::Unsetenv(key);
}
void Clearenv()
{
    syscall::Clearenv();
}
moku::slice<std::string> Environ()
{
    return syscall::Environ();
}
void init();
int main()
{
    init();
    ErrInvalid = errors::New("invalid argument");
    ErrPermission = errors::New("permission denied");
    ErrExist = errors::New("file already exists");
    ErrNotExist = errors::New("file does not exist");
    ErrClosed = errors::New("file already closed");
    Interrupt = syscall::SIGINT;
    Kill = syscall::SIGKILL;
    errFinished = errors::New("os: process already finished");
    Stdin = NewFile(uintptr_t(syscall::Stdin), "/dev/stdin");
    Stdout = NewFile(uintptr_t(syscall::Stdout), "/dev/stdout");
    Stderr = NewFile(uintptr_t(syscall::Stderr), "/dev/stderr");
    useSyscallwd = [=](moku::error) mutable -> bool { return true; };
    lstat = Lstat;
    std::tie(initCwd, initCwdErr) = Getwd();
    _main();
    return 0;
}
std::string PathError::Error()
{
    return this->Op + " " + this->Path + ": " + this->Err.Error();
}
std::string SyscallError::Error()
{
    return this->Syscall + ": " + this->Err.Error();
}
moku::error NewSyscallError(std::string syscall, moku::error err)
{
    if (moku::is_nil(err)) {
        return nil;
    }
    return &SyscallError{syscall, err};
}
bool IsExist(moku::error err)
{
    return isExist(err);
}
bool IsNotExist(moku::error err)
{
    return isNotExist(err);
}
bool IsPermission(moku::error err)
{
    return isPermission(err);
}
moku::error underlyingError(moku::error err)
{
}
