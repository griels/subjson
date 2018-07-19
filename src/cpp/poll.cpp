moku::error errClosing(bool isFile)
{
    if (isFile) {
        return ErrFileClosing;
    }
    return ErrNetClosing;
}
std::string TimeoutError::Error()
{
    return "i/o timeout";
}
bool TimeoutError::Timeout()
{
    return true;
}
bool TimeoutError::Temporary()
{
    return true;
}
void consume(moku::slice<moku::slice<uint8_t>> *v, int64_t n)
{
    while (len(*v) > 0) {
        int64_t ln0{0};

        ln0 = int64_t(len((*v)[0]));
        if (ln0 > n) {
            (*v)[0] = moku::slice_expr<moku::slice<uint8_t>>((*v)[0], n);
            return;
        }
        n -= ln0;
        *v = moku::slice_expr<moku::slice<moku::slice<uint8_t>>>((*v), 1);
    }
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
bool fdMutex::incref()
{
    while (true) {
        uint64_t new {0};
        uint64_t old{0};

        old = atomic::LoadUint64(&this->state);
        if (old & mutexClosed != 0) {
            return false;
        }
        new = old + mutexRef;
        if (new &mutexRefMask == 0) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 4397900) STRING
                         "inconsistent poll.fdMutex"}));
        }
        if (atomic::CompareAndSwapUint64(&this->state, old, new)) {
            return true;
        }
    }
}
bool fdMutex::increfAndClose()
{
    while (true) {
        uint64_t new {0};
        uint64_t old{0};

        old = atomic::LoadUint64(&this->state);
        if (old & mutexClosed != 0) {
            return false;
        }
        new = (old | mutexClosed) + mutexRef;
        if (new &mutexRefMask == 0) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 4398378) STRING
                         "inconsistent poll.fdMutex"}));
        }
        new &= ~(mutexRMask | mutexWMask);
        if (atomic::CompareAndSwapUint64(&this->state, old, new)) {
            while (old & mutexRMask != 0) {
                old -= mutexRWait;
                runtime_Semrelease(&this->rsema);
            }
            while (old & mutexWMask != 0) {
                old -= mutexWWait;
                runtime_Semrelease(&this->wsema);
            }
            return true;
        }
    }
}
bool fdMutex::decref()
{
    while (true) {
        uint64_t new {0};
        uint64_t old{0};

        old = atomic::LoadUint64(&this->state);
        if (old & mutexRefMask == 0) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 4399047) STRING
                         "inconsistent poll.fdMutex"}));
        }
        new = old - mutexRef;
        if (atomic::CompareAndSwapUint64(&this->state, old, new)) {
            return new &(mutexClosed | mutexRefMask) == mutexClosed;
        }
    }
}
bool fdMutex::rwlock(bool read)
{
    uint64_t mutexBit{0};
    uint64_t mutexMask{0};
    uint32_t *mutexSema{std::nullptr};
    uint64_t mutexWait{0};

    if (read) {
        mutexBit = mutexRLock;
        mutexWait = mutexRWait;
        mutexMask = mutexRMask;
        mutexSema = &this->rsema;
    } else {
        mutexBit = mutexWLock;
        mutexWait = mutexWWait;
        mutexMask = mutexWMask;
        mutexSema = &this->wsema;
    }
    while (true) {
        uint64_t new {0};
        uint64_t old{0};

        old = atomic::LoadUint64(&this->state);
        if (old & mutexClosed != 0) {
            return false;
        }
        if (old & mutexBit == 0) {
            new = (old | mutexBit) + mutexRef;
            if (new &mutexRefMask == 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 4399907) STRING
                             "inconsistent poll.fdMutex"}));
            }
        } else {
            new = old + mutexWait;
            if (new &mutexMask == 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 4400035) STRING
                             "inconsistent poll.fdMutex"}));
            }
        }
        if (atomic::CompareAndSwapUint64(&this->state, old, new)) {
            if (old & mutexBit == 0) {
                return true;
            }
            runtime_Semacquire(mutexSema);
        }
    }
}
bool fdMutex::rwunlock(bool read)
{
    uint64_t mutexBit{0};
    uint64_t mutexMask{0};
    uint32_t *mutexSema{std::nullptr};
    uint64_t mutexWait{0};

    if (read) {
        mutexBit = mutexRLock;
        mutexWait = mutexRWait;
        mutexMask = mutexRMask;
        mutexSema = &this->rsema;
    } else {
        mutexBit = mutexWLock;
        mutexWait = mutexWWait;
        mutexMask = mutexWMask;
        mutexSema = &this->wsema;
    }
    while (true) {
        uint64_t new {0};
        uint64_t old{0};

        old = atomic::LoadUint64(&this->state);
        if (old & mutexBit == 0 || old & mutexRefMask == 0) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 4400810) STRING
                         "inconsistent poll.fdMutex"}));
        }
        new = (old & ~(mutexBit)) - mutexRef;
        if (old & mutexMask != 0) {
            new -= mutexWait;
        }
        if (atomic::CompareAndSwapUint64(&this->state, old, new)) {
            if (old & mutexMask != 0) {
                runtime_Semrelease(mutexSema);
            }
            return new &(mutexClosed | mutexRefMask) == mutexClosed;
        }
    }
}
void runtime_Semacquire(uint32_t *sema)
{
}
void runtime_Semrelease(uint32_t *sema)
{
}
moku::error FD::incref()
{
    if (!this->fdmu.incref()) {
        return errClosing(this->isFile);
    }
    return nil;
}
moku::error FD::decref()
{
    if (this->fdmu.decref()) {
        return this->destroy();
    }
    return nil;
}
moku::error FD::readLock()
{
    if (!this->fdmu.rwlock(true)) {
        return errClosing(this->isFile);
    }
    return nil;
}
void FD::readUnlock()
{
    if (this->fdmu.rwunlock(true)) {
        this->destroy();
    }
}
moku::error FD::writeLock()
{
    if (!this->fdmu.rwlock(false)) {
        return errClosing(this->isFile);
    }
    return nil;
}
void FD::writeUnlock()
{
    if (this->fdmu.rwunlock(false)) {
        this->destroy();
    }
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
int64_t runtimeNano()
{
}
void runtime_pollServerInit()
{
}
uintptr_t runtime_pollServerDescriptor()
{
}
std::tuple<uintptr_t, int> runtime_pollOpen(uintptr_t fd)
{
}
void runtime_pollClose(uintptr_t ctx)
{
}
int runtime_pollWait(uintptr_t ctx, int mode)
{
}
int runtime_pollWaitCanceled(uintptr_t ctx, int mode)
{
}
int runtime_pollReset(uintptr_t ctx, int mode)
{
}
void runtime_pollSetDeadline(uintptr_t ctx, int64_t d, int mode)
{
}
void runtime_pollUnblock(uintptr_t ctx)
{
}
moku::error pollDesc::init(FD *fd)
{
    uintptr_t ctx{0};
    int errno{0};

    serverInit.Do(runtime_pollServerInit);
    std::tie(ctx, errno) = runtime_pollOpen(uintptr_t(fd->Sysfd));
    if (errno != 0) {
        if (ctx != 0) {
            runtime_pollUnblock(ctx);
            runtime_pollClose(ctx);
        }
        return syscall::Errno(errno);
    }
    this->runtimeCtx = ctx;
    return nil;
}
void pollDesc::close()
{
    if (this->runtimeCtx == 0) {
        return;
    }
    runtime_pollClose(this->runtimeCtx);
    this->runtimeCtx = 0;
}
void pollDesc::evict()
{
    if (this->runtimeCtx == 0) {
        return;
    }
    runtime_pollUnblock(this->runtimeCtx);
}
moku::error pollDesc::prepare(int mode, bool isFile)
{
    int res{0};

    if (this->runtimeCtx == 0) {
        return nil;
    }
    res = runtime_pollReset(this->runtimeCtx, mode);
    return convertErr(res, isFile);
}
moku::error pollDesc::prepareRead(bool isFile)
{
    return this->prepare('r', isFile);
}
moku::error pollDesc::prepareWrite(bool isFile)
{
    return this->prepare('w', isFile);
}
moku::error pollDesc::wait(int mode, bool isFile)
{
    int res{0};

    if (this->runtimeCtx == 0) {
        return errors::New("waiting for unsupported file type");
    }
    res = runtime_pollWait(this->runtimeCtx, mode);
    return convertErr(res, isFile);
}
moku::error pollDesc::waitRead(bool isFile)
{
    return this->wait('r', isFile);
}
moku::error pollDesc::waitWrite(bool isFile)
{
    return this->wait('w', isFile);
}
void pollDesc::waitCanceled(int mode)
{
    if (this->runtimeCtx == 0) {
        return;
    }
    runtime_pollWaitCanceled(this->runtimeCtx, mode);
}
bool pollDesc::pollable()
{
    return this->runtimeCtx != 0;
}
moku::error convertErr(int res, bool isFile)
{
    if ((res == 0)) {
    _ident_0_:
        return nil;
    } else if ((res == 1)) {
    _ident_1_:
        return errClosing(isFile);
    } else if ((res == 2)) {
    _ident_2_:
        return ErrTimeout;
    }
    println("unreachable: ", res);
    panic(moku::make_iface<std::string>(
            &{ % !s(token.Pos = 4394880) STRING "unreachable"}));
}
moku::error FD::SetDeadline(Time t)
{
    return setDeadlineImpl(this, t, 'r' + 'w');
}
moku::error FD::SetReadDeadline(Time t)
{
    return setDeadlineImpl(this, t, 'r');
}
moku::error FD::SetWriteDeadline(Time t)
{
    return setDeadlineImpl(this, t, 'w');
}
moku::error setDeadlineImpl(FD *fd, Time t, int mode)
{
    int64_t d{0};
    int64_t diff{0};

    diff = int64_t(time::Until(t));
    d = runtimeNano() + diff;
    if (d <= 0 && diff > 0) {
        d = 1 << 63 - 1;
    }
    if (t.IsZero()) {
        d = 0;
    }
    {
        moku::error err{};

        err = fd->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    if (fd->pd.runtimeCtx == 0) {
        return errors::New("file type does not support deadlines");
    }
    runtime_pollSetDeadline(fd->pd.runtimeCtx, d, mode);
    fd->decref();
    return nil;
}
uintptr_t PollDescriptor()
{
    return runtime_pollServerDescriptor();
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
moku::error FD::eofError(int n, moku::error err)
{
    if (n == 0 && moku::is_nil(err) && this->ZeroReadIsEOF) {
        return io::EOF;
    }
    return err;
}
moku::error FD::Fchmod(uint32_t mode)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Fchmod(this->Sysfd, mode);
}
moku::error FD::Fchown(int uid, int gid)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Fchown(this->Sysfd, uid, gid);
}
moku::error FD::Ftruncate(int64_t size)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Ftruncate(this->Sysfd, size);
}
moku::error FD::Fsync()
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Fsync(this->Sysfd);
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
moku::error FD::Init(std::string net, bool pollable)
{
    if (net == "file") {
        this->isFile = true;
    }
    if (!pollable) {
        return nil;
    }
    return this->pd.init(this);
}
moku::error FD::destroy()
{
    moku::error err{};

    this->pd.close();
    err = CloseFunc(this->Sysfd);
    this->Sysfd = -1;
    return err;
}
moku::error FD::Close()
{
    if (!this->fdmu.increfAndClose()) {
        return errClosing(this->isFile);
    }
    this->pd.evict();
    return this->decref();
}
moku::error FD::Shutdown(int how)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Shutdown(this->Sysfd, how);
}
std::tuple<int, moku::error> FD::Read(moku::slice<uint8_t> p)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->readLock();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    _defer_.Push([=]() mutable { this->readUnlock(); });
    if (len(p) == 0) {
        return {0, nil};
    }
    {
        moku::error err{};

        err = this->pd.prepareRead(this->isFile);

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    if (this->IsStream && len(p) > maxRW) {
        p = moku::slice_expr<moku::slice<uint8_t>>(p, maxRW);
    }
    while (true) {
        moku::error err{};
        int n{0};

        std::tie(n, err) = syscall::Read(this->Sysfd, p);
        if (!moku::is_nil(err)) {
            n = 0;
            if (err == syscall::EAGAIN && this->pd.pollable()) {
                {
                    err = this->pd.waitRead(this->isFile);

                    if (moku::is_nil(err)) {
                        continue;
                    }
                }
            }
        }
        err = this->eofError(n, err);
        return {n, err};
    }
}
std::tuple<int, moku::error> FD::Pread(moku::slice<uint8_t> p, int64_t off)
{
    moku::error err{};
    int n{0};

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    if (this->IsStream && len(p) > maxRW) {
        p = moku::slice_expr<moku::slice<uint8_t>>(p, maxRW);
    }
    std::tie(n, err) = syscall::Pread(this->Sysfd, p, off);
    if (!moku::is_nil(err)) {
        n = 0;
    }
    this->decref();
    err = this->eofError(n, err);
    return {n, err};
}
std::tuple<int, Sockaddr, moku::error> FD::ReadFrom(moku::slice<uint8_t> p)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->readLock();

        if (!moku::is_nil(err)) {
            return {0, nil, err};
        }
    }
    _defer_.Push([=]() mutable { this->readUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareRead(this->isFile);

        if (!moku::is_nil(err)) {
            return {0, nil, err};
        }
    }
    while (true) {
        moku::error err{};
        int n{0};
        Sockaddr sa{};

        std::tie(n, sa, err) = syscall::Recvfrom(this->Sysfd, p, 0);
        if (!moku::is_nil(err)) {
            n = 0;
            if (err == syscall::EAGAIN && this->pd.pollable()) {
                {
                    err = this->pd.waitRead(this->isFile);

                    if (moku::is_nil(err)) {
                        continue;
                    }
                }
            }
        }
        err = this->eofError(n, err);
        return {n, sa, err};
    }
}
std::tuple<int, int, int, Sockaddr, moku::error> FD::ReadMsg(
        moku::slice<uint8_t> p, moku::slice<uint8_t> oob)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->readLock();

        if (!moku::is_nil(err)) {
            return {0, 0, 0, nil, err};
        }
    }
    _defer_.Push([=]() mutable { this->readUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareRead(this->isFile);

        if (!moku::is_nil(err)) {
            return {0, 0, 0, nil, err};
        }
    }
    while (true) {
        moku::error err{};
        int flags{0};
        int n{0};
        int oobn{0};
        Sockaddr sa{};

        std::tie(n, oobn, flags, sa, err) =
                syscall::Recvmsg(this->Sysfd, p, oob, 0);
        if (!moku::is_nil(err)) {
            if (err == syscall::EAGAIN && this->pd.pollable()) {
                {
                    err = this->pd.waitRead(this->isFile);

                    if (moku::is_nil(err)) {
                        continue;
                    }
                }
            }
        }
        err = this->eofError(n, err);
        return {n, oobn, flags, sa, err};
    }
}
std::tuple<int, moku::error> FD::Write(moku::slice<uint8_t> p)
{
    moku::defer _defer_;
    int nn{0};

    {
        moku::error err{};

        err = this->writeLock();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    _defer_.Push([=]() mutable { this->writeUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareWrite(this->isFile);

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    while (true) {
        moku::error err{};
        int max{0};
        int n{0};

        max = len(p);
        if (this->IsStream && max - nn > maxRW) {
            max = nn + maxRW;
        }
        std::tie(n, err) = syscall::Write(
                this->Sysfd,
                moku::slice_expr<moku::slice<uint8_t>>(p, nn, max));
        if (n > 0) {
            nn += n;
        }
        if (nn == len(p)) {
            return {nn, err};
        }
        if (err == syscall::EAGAIN && this->pd.pollable()) {
            {
                err = this->pd.waitWrite(this->isFile);

                if (moku::is_nil(err)) {
                    continue;
                }
            }
        }
        if (!moku::is_nil(err)) {
            return {nn, err};
        }
        if (n == 0) {
            return {nn, io::ErrUnexpectedEOF};
        }
    }
}
std::tuple<int, moku::error> FD::Pwrite(moku::slice<uint8_t> p, int64_t off)
{
    moku::defer _defer_;
    int nn{0};

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    while (true) {
        moku::error err{};
        int max{0};
        int n{0};

        max = len(p);
        if (this->IsStream && max - nn > maxRW) {
            max = nn + maxRW;
        }
        std::tie(n, err) = syscall::Pwrite(
                this->Sysfd,
                moku::slice_expr<moku::slice<uint8_t>>(p, nn, max),
                off + int64_t(nn));
        if (n > 0) {
            nn += n;
        }
        if (nn == len(p)) {
            return {nn, err};
        }
        if (!moku::is_nil(err)) {
            return {nn, err};
        }
        if (n == 0) {
            return {nn, io::ErrUnexpectedEOF};
        }
    }
}
std::tuple<int, moku::error> FD::WriteTo(moku::slice<uint8_t> p, Sockaddr sa)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->writeLock();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    _defer_.Push([=]() mutable { this->writeUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareWrite(this->isFile);

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    while (true) {
        moku::error err{};

        err = syscall::Sendto(this->Sysfd, p, 0, sa);
        if (err == syscall::EAGAIN && this->pd.pollable()) {
            {
                err = this->pd.waitWrite(this->isFile);

                if (moku::is_nil(err)) {
                    continue;
                }
            }
        }
        if (!moku::is_nil(err)) {
            return {0, err};
        }
        return {len(p), nil};
    }
}
std::tuple<int, int, moku::error> FD::WriteMsg(moku::slice<uint8_t> p,
                                               moku::slice<uint8_t> oob,
                                               Sockaddr sa)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->writeLock();

        if (!moku::is_nil(err)) {
            return {0, 0, err};
        }
    }
    _defer_.Push([=]() mutable { this->writeUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareWrite(this->isFile);

        if (!moku::is_nil(err)) {
            return {0, 0, err};
        }
    }
    while (true) {
        moku::error err{};
        int n{0};

        std::tie(n, err) = syscall::SendmsgN(this->Sysfd, p, oob, sa, 0);
        if (err == syscall::EAGAIN && this->pd.pollable()) {
            {
                err = this->pd.waitWrite(this->isFile);

                if (moku::is_nil(err)) {
                    continue;
                }
            }
        }
        if (!moku::is_nil(err)) {
            return {n, 0, err};
        }
        return {n, len(oob), err};
    }
}
std::tuple<int, Sockaddr, std::string, moku::error> FD::Accept()
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->readLock();

        if (!moku::is_nil(err)) {
            return {-1, nil, "", err};
        }
    }
    _defer_.Push([=]() mutable { this->readUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareRead(this->isFile);

        if (!moku::is_nil(err)) {
            return {-1, nil, "", err};
        }
    }
    while (true) {
        moku::error err{};
        std::string errcall{""};
        Sockaddr rsa{};
        int s{0};

        std::tie(s, rsa, errcall, err) = accept(this->Sysfd);
        if (moku::is_nil(err)) {
            return {s, rsa, "", err};
        }
        if ((err == syscall::EAGAIN)) {
        _ident_0_:
            if (this->pd.pollable()) {
                {
                    err = this->pd.waitRead(this->isFile);

                    if (moku::is_nil(err)) {
                        continue;
                    }
                }
            }
        } else if ((err == syscall::ECONNABORTED)) {
        _ident_1_:
            continue;
        }
        return {-1, nil, errcall, err};
    }
}
std::tuple<int64_t, moku::error> FD::Seek(int64_t offset, int whence)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Seek(this->Sysfd, offset, whence);
}
std::tuple<int, moku::error> FD::ReadDirent(moku::slice<uint8_t> buf)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    while (true) {
        moku::error err{};
        int n{0};

        std::tie(n, err) = syscall::ReadDirent(this->Sysfd, buf);
        if (!moku::is_nil(err)) {
            n = 0;
            if (err == syscall::EAGAIN && this->pd.pollable()) {
                {
                    err = this->pd.waitRead(this->isFile);

                    if (moku::is_nil(err)) {
                        continue;
                    }
                }
            }
        }
        return {n, err};
    }
}
moku::error FD::Fchdir()
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Fchdir(this->Sysfd);
}
moku::error FD::Fstat(Stat_t *s)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::Fstat(this->Sysfd, s);
}
moku::error FD::WaitWrite()
{
    return this->pd.waitWrite(this->isFile);
}
moku::error FD::RawControl(std::function<void(uintptr_t)> f)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    f(uintptr_t(this->Sysfd));
    return nil;
}
moku::error FD::RawRead(std::function<bool(uintptr_t)> f)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->readLock();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->readUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareRead(this->isFile);

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    while (true) {
        if (f(uintptr_t(this->Sysfd))) {
            return nil;
        }
        {
            moku::error err{};

            err = this->pd.waitRead(this->isFile);

            if (!moku::is_nil(err)) {
                return err;
            }
        }
    }
}
moku::error FD::RawWrite(std::function<bool(uintptr_t)> f)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->writeLock();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->writeUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareWrite(this->isFile);

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    while (true) {
        if (f(uintptr_t(this->Sysfd))) {
            return nil;
        }
        {
            moku::error err{};

            err = this->pd.waitWrite(this->isFile);

            if (!moku::is_nil(err)) {
                return err;
            }
        }
    }
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
moku::error FD::SetsockoptInt(int level, int name, int arg)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::SetsockoptInt(this->Sysfd, level, name, arg);
}
moku::error FD::SetsockoptInet4Addr(int level,
                                    int name,
                                    std::vector<uint8_t> arg)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::SetsockoptInet4Addr(this->Sysfd, level, name, arg);
}
moku::error FD::SetsockoptLinger(int level, int name, Linger *l)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::SetsockoptLinger(this->Sysfd, level, name, l);
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
moku::error FD::SetsockoptByte(int level, int name, uint8_t arg)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::SetsockoptByte(this->Sysfd, level, name, arg);
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
moku::error FD::SetsockoptIPMreq(int level, int name, IPMreq *mreq)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::SetsockoptIPMreq(this->Sysfd, level, name, mreq);
}
moku::error FD::SetsockoptIPv6Mreq(int level, int name, IPv6Mreq *mreq)
{
    moku::defer _defer_;

    {
        moku::error err{};

        err = this->incref();

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    _defer_.Push([=]() mutable { this->decref(); });
    return syscall::SetsockoptIPv6Mreq(this->Sysfd, level, name, mreq);
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
std::tuple<int, Sockaddr, std::string, moku::error> accept(int s)
{
    moku::error err{};
    int ns{0};
    Sockaddr sa{};

    std::tie(ns, sa, err) = AcceptFunc(s);
    if (moku::is_nil(err)) {
        syscall::CloseOnExec(ns);
    }
    if (!moku::is_nil(err)) {
        return {-1, nil, "accept", err};
    }
    {
        err = syscall::SetNonblock(ns, true);

        if (!moku::is_nil(err)) {
            CloseFunc(ns);
            return {-1, nil, "setnonblock", err};
        }
    }
    return {ns, sa, "", nil};
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
std::tuple<int64_t, moku::error> FD::Writev(
        moku::slice<moku::slice<uint8_t>> *v)
{
    moku::defer _defer_;
    moku::error err{};
    moku::slice<Iovec> iovecs{};
    int maxVec{0};
    int64_t n{0};

    {
        moku::error err{};

        err = this->writeLock();

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    _defer_.Push([=]() mutable { this->writeUnlock(); });
    {
        moku::error err{};

        err = this->pd.prepareWrite(this->isFile);

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    if (!moku::is_nil(this->iovecs)) {
        iovecs = *this->iovecs;
    }
    maxVec = 1024;
    while (len(*v) > 0) {
        Errno e0{};
        uintptr_t wrote{0};

        iovecs = moku::slice_expr<moku::slice<Iovec>>(iovecs, 0);
        {
            moku::slice<uint8_t> chunk{};
            for (chunk :
                 moku::range_value<moku::slice<moku::slice<uint8_t>>>(*v)) {
                if (len(chunk) == 0) {
                    continue;
                }
                iovecs = append(iovecs, syscall::Iovec{{Base, &chunk[0]}});
                if (this->IsStream && len(chunk) > 1 << 30) {
                    iovecs[len(iovecs) - 1].SetLen(1 << 30);
                    break;
                }
                iovecs[len(iovecs) - 1].SetLen(len(chunk));
                if (len(iovecs) == maxVec) {
                    break;
                }
            }
        }
        if (len(iovecs) == 0) {
            break;
        }
        this->iovecs = &iovecs;
        std::tie(wrote, _, e0) =
                syscall::Syscall(syscall::SYS_WRITEV,
                                 uintptr_t(this->Sysfd),
                                 uintptr_t(unsafe::Pointer(&iovecs[0])),
                                 uintptr_t(len(iovecs)));
        if (wrote == ^uintptr_t(0)) {
            wrote = 0;
        }
        TestHookDidWritev(int(wrote));
        n += int64_t(wrote);
        consume(v, int64_t(wrote));
        if (e0 == syscall::EAGAIN) {
            {
                err = this->pd.waitWrite(this->isFile);

                if (moku::is_nil(err)) {
                    continue;
                }
            }
        } else {
            if (e0 != 0) {
                err = syscall::Errno(e0);
            }
        }
        if (!moku::is_nil(err)) {
            break;
        }
        if (n == 0) {
            err = io::ErrUnexpectedEOF;
            break;
        }
    }
    return {n, err};
}
void init();
int main()
{
    init();
    ErrNetClosing = errors::New("use of closed network connection");
    ErrFileClosing = errors::New("use of closed file");
    ErrTimeout = &TimeoutError{};
    TestHookDidWritev = [=](int wrote) mutable -> void {};
    CloseFunc = syscall::Close;
    AcceptFunc = syscall::Accept;
    _main();
    return 0;
}
