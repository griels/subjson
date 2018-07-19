Cond *NewCond(Locker l)
{
    return &Cond{{L, l}};
}
void Cond::Wait()
{
    uint32_t t{0};

    this->checker.check();
    t = runtime_notifyListAdd(&this->notify);
    this->L.Unlock();
    runtime_notifyListWait(&this->notify, t);
    this->L.Lock();
}
void Cond::Signal()
{
    this->checker.check();
    runtime_notifyListNotifyOne(&this->notify);
}
void Cond::Broadcast()
{
    this->checker.check();
    runtime_notifyListNotifyAll(&this->notify);
}
void copyChecker::check()
{
    if (uintptr_t(*this) != uintptr_t(unsafe::Pointer(this)) &&
        !atomic::CompareAndSwapUintptr(
                (*uintptr_t)(this), 0, uintptr_t(unsafe::Pointer(this))) &&
        uintptr_t(*this) != uintptr_t(unsafe::Pointer(this))) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 338077) STRING "sync.Cond is copied"}));
    }
}
void noCopy::Lock()
{
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
entry *newEntry(moku::interface i)
{
    return &entry{{p, unsafe::Pointer(&i)}};
}
std::tuple<moku::interface, bool> Map::Load(moku::interface key)
{
    entry *e{std::nullptr};
    bool ok{false};
    readOnly read{};
    moku::interface value{};

    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    std::tie(e, ok) = read.m[key];
    if (!ok && read.amended) {
        this->mu.Lock();
        std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
        std::tie(e, ok) = read.m[key];
        if (!ok && read.amended) {
            std::tie(e, ok) = this->dirty[key];
            this->missLocked();
        }
        this->mu.Unlock();
    }
    if (!ok) {
        return {nil, false};
    }
    return e->load();
}
std::tuple<moku::interface, bool> entry::load()
{
    bool ok{false};
    void *p{std::nullptr};
    moku::interface value{};

    p = atomic::LoadPointer(&this->p);
    if (moku::is_nil(p) || p == expunged) {
        return {nil, false};
    }
    return {*(*moku::empty_interface)(p), true};
}
void Map::Store(moku::interface key, moku::interface value)
{
    readOnly read{};

    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    {
        entry *e{std::nullptr};
        bool ok{false};

        std::tie(e, ok) = read.m[key];

        if (ok && e->tryStore(&value)) {
            return;
        }
    }
    this->mu.Lock();
    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    {
        entry *e{std::nullptr};
        bool ok{false};

        std::tie(e, ok) = read.m[key];

        if (ok) {
            if (e->unexpungeLocked()) {
                this->dirty[key] = e;
            }
            e->storeLocked(&value);
        } else {
            {
                entry *e{std::nullptr};
                bool ok{false};

                std::tie(e, ok) = this->dirty[key];

                if (ok) {
                    e->storeLocked(&value);
                } else {
                    if (!read.amended) {
                        this->dirtyLocked();
                        this->read.Store(moku::make_iface<readOnly>(&{
                                readOnly %
                                !s(token.Pos = 349502)
                                        [%
                                         !s(*ast.KeyValueExpr =
                                                    &{0xc4201f8720 349504 0xc4201f8780}) %
                                         !s(*ast.KeyValueExpr =
                                                    &{0xc4201f87a0 349521 0xc4201f87c0})] %
                                !s(token.Pos = 349527)}));
                    }
                    this->dirty[key] =
                            newEntry(moku::make_iface<moku::interface>(value));
                }
            }
        }
    }
    this->mu.Unlock();
}
bool entry::tryStore(moku::interface *i)
{
    void *p{std::nullptr};

    p = atomic::LoadPointer(&this->p);
    if (p == expunged) {
        return false;
    }
    while (true) {
        if (atomic::CompareAndSwapPointer(&this->p, p, unsafe::Pointer(i))) {
            return true;
        }
        p = atomic::LoadPointer(&this->p);
        if (p == expunged) {
            return false;
        }
    }
}
bool entry::unexpungeLocked()
{
    bool wasExpunged{false};

    return atomic::CompareAndSwapPointer(&this->p, expunged, nil);
}
void entry::storeLocked(moku::interface *i)
{
    atomic::StorePointer(&this->p, unsafe::Pointer(i));
}
std::tuple<moku::interface, bool> Map::LoadOrStore(moku::interface key,
                                                   moku::interface value)
{
    moku::interface actual{};
    bool loaded{false};
    readOnly read{};

    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    {
        entry *e{std::nullptr};
        bool ok{false};

        std::tie(e, ok) = read.m[key];

        if (ok) {
            std::tie(actual, loaded, ok) =
                    e->tryLoadOrStore(moku::make_iface<moku::interface>(value));
            if (ok) {
                return {actual, loaded};
            }
        }
    }
    this->mu.Lock();
    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    {
        entry *e{std::nullptr};
        bool ok{false};

        std::tie(e, ok) = read.m[key];

        if (ok) {
            if (e->unexpungeLocked()) {
                this->dirty[key] = e;
            }
            std::tie(actual, loaded, _) =
                    e->tryLoadOrStore(moku::make_iface<moku::interface>(value));
        } else {
            {
                entry *e{std::nullptr};
                bool ok{false};

                std::tie(e, ok) = this->dirty[key];

                if (ok) {
                    std::tie(actual, loaded, _) = e->tryLoadOrStore(
                            moku::make_iface<moku::interface>(value));
                    this->missLocked();
                } else {
                    if (!read.amended) {
                        this->dirtyLocked();
                        this->read.Store(moku::make_iface<readOnly>(&{
                                readOnly %
                                !s(token.Pos = 351478)
                                        [%
                                         !s(*ast.KeyValueExpr =
                                                    &{0xc4203606a0 351480 0xc420360700}) %
                                         !s(*ast.KeyValueExpr =
                                                    &{0xc420360720 351497 0xc420360740})] %
                                !s(token.Pos = 351503)}));
                    }
                    this->dirty[key] =
                            newEntry(moku::make_iface<moku::interface>(value));
                    std::tie(actual, loaded) =
                            std::tuple<moku::interface, bool>(value, false);
                }
            }
        }
    }
    this->mu.Unlock();
    return {actual, loaded};
}
std::tuple<moku::interface, bool, bool> entry::tryLoadOrStore(moku::interface i)
{
    moku::interface actual{};
    moku::interface ic{};
    bool loaded{false};
    bool ok{false};
    void *p{std::nullptr};

    p = atomic::LoadPointer(&this->p);
    if (p == expunged) {
        return {nil, false, false};
    }
    if (!moku::is_nil(p)) {
        return {*(*moku::empty_interface)(p), true, true};
    }
    ic = i;
    while (true) {
        if (atomic::CompareAndSwapPointer(
                    &this->p, nil, unsafe::Pointer(&ic))) {
            return {i, false, true};
        }
        p = atomic::LoadPointer(&this->p);
        if (p == expunged) {
            return {nil, false, false};
        }
        if (!moku::is_nil(p)) {
            return {*(*moku::empty_interface)(p), true, true};
        }
    }
}
void Map::Delete(moku::interface key)
{
    entry *e{std::nullptr};
    bool ok{false};
    readOnly read{};

    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    std::tie(e, ok) = read.m[key];
    if (!ok && read.amended) {
        this->mu.Lock();
        std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
        std::tie(e, ok) = read.m[key];
        if (!ok && read.amended) {
            delete (this->dirty, moku::make_iface<moku::interface>(key));
        }
        this->mu.Unlock();
    }
    if (ok) {
        e->delete ();
    }
}
bool entry::delete ()
{
    bool hadValue{false};

    while (true) {
        void *p{std::nullptr};

        p = atomic::LoadPointer(&this->p);
        if (moku::is_nil(p) || p == expunged) {
            return false;
        }
        if (atomic::CompareAndSwapPointer(&this->p, p, nil)) {
            return true;
        }
    }
}
void Map::Range(std::function<bool(moku::interface, moku::interface)> f)
{
    readOnly read{};

    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    if (read.amended) {
        this->mu.Lock();
        std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
        if (read.amended) {
            read = readOnly{this : this->dirty};
            this->read.Store(moku::make_iface<readOnly>(read));
            this->dirty = nil;
            this->misses = 0;
        }
        this->mu.Unlock();
    }
    {
        entry *e{std::nullptr};
        moku::interface k{};
        for (std::tie(k, e) :
             moku::range_key_value<std::map<moku::interface, entry *>>(
                     read.m)) {
            std::tie(v, ok) = e->load();
            if (!ok) {
                continue;
            }
            if (!f(moku::make_iface<moku::interface>(k),
                   moku::make_iface<moku::interface>(v))) {
                break;
            }
        }
    }
}
void Map::missLocked()
{
    this->misses++;
    if (this->misses < len(this->dirty)) {
        return;
    }
    this->read.Store(moku::make_iface<readOnly>(
            &{readOnly %
              !s(token.Pos = 354570)[% !s(
                              *ast.KeyValueExpr =
                                      &{0xc4202fb0e0 354572 0xc4202fb140})] %
              !s(token.Pos = 354581)}));
    this->dirty = nil;
    this->misses = 0;
}
void Map::dirtyLocked()
{
    readOnly read{};

    if (!moku::is_nil(this->dirty)) {
        return;
    }
    std::tie(read, _) = moku::try_assert<readOnly>(this->read.Load());
    this->dirty = make(std::map<moku::empty_interface, *entry>, len(read.m));
    {
        entry *e{std::nullptr};
        moku::interface k{};
        for (std::tie(k, e) :
             moku::range_key_value<std::map<moku::interface, entry *>>(
                     read.m)) {
            if (!e->tryExpungeLocked()) {
                this->dirty[k] = e;
            }
        }
    }
}
bool entry::tryExpungeLocked()
{
    bool isExpunged{false};
    void *p{std::nullptr};

    p = atomic::LoadPointer(&this->p);
    while (moku::is_nil(p)) {
        if (atomic::CompareAndSwapPointer(&this->p, nil, expunged)) {
            return true;
        }
        p = atomic::LoadPointer(&this->p);
    }
    return p == expunged;
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
void throw(std::string)
{
}
void Mutex::Lock()
{
    bool awoke{false};
    int iter{0};
    int32_t old{0};
    bool starving{false};
    int64_t waitStartTime{0};

    if (atomic::CompareAndSwapInt32(&this->state, 0, mutexLocked)) {
        if (race::Enabled) {
            race::Acquire(unsafe::Pointer(this));
        }
        return;
    }
    starving = false;
    awoke = false;
    iter = 0;
    old = this->state;
    while (true) {
        int32_t new {0};

        if (old & (mutexLocked | mutexStarving) == mutexLocked &&
            runtime_canSpin(iter)) {
            if (!awoke && old & mutexWoken == 0 &&
                old >> mutexWaiterShift != 0 &&
                atomic::CompareAndSwapInt32(
                        &this->state, old, old | mutexWoken)) {
                awoke = true;
            }
            runtime_doSpin();
            iter++;
            old = this->state;
            continue;
        }
        new = old;
        if (old & mutexStarving == 0) {
            new |= mutexLocked;
        }
        if (old & (mutexLocked | mutexStarving) != 0) {
            new += 1 << mutexWaiterShift;
        }
        if (starving && old & mutexLocked != 0) {
            new |= mutexStarving;
        }
        if (awoke) {
            if (new &mutexWoken == 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 372984) STRING
                             "sync: inconsistent mutex state"}));
            }
            new &= ~(mutexWoken);
        }
        if (atomic::CompareAndSwapInt32(&this->state, old, new)) {
            if (old & (mutexLocked | mutexStarving) == 0) {
                break;
            }
            queueLifo = waitStartTime != 0;
            if (waitStartTime == 0) {
                waitStartTime = runtime_nanotime();
            }
            runtime_SemacquireMutex(&this->sema, queueLifo);
            starving = starving || runtime_nanotime() - waitStartTime >
                                           starvationThresholdNs;
            old = this->state;
            if (old & mutexStarving != 0) {
                if (old & (mutexLocked | mutexWoken) != 0 ||
                    old >> mutexWaiterShift == 0) {
                    panic(moku::make_iface<std::string>(
                            &{ % !s(token.Pos = 373870) STRING
                                 "sync: inconsistent mutex state"}));
                }
                delta = int32_t(mutexLocked - 1 << mutexWaiterShift);
                if (!starving || old >> mutexWaiterShift == 1) {
                    delta -= mutexStarving;
                }
                atomic::AddInt32(&this->state, delta);
                break;
            }
            awoke = true;
            iter = 0;
        } else {
            old = this->state;
        }
    }
    if (race::Enabled) {
        race::Acquire(unsafe::Pointer(this));
    }
}
void Mutex::Unlock()
{
    int32_t new {0};

    if (race::Enabled) {
        _ = this->state;
        race::Release(unsafe::Pointer(this));
    }
    new = atomic::AddInt32(&this->state, -mutexLocked);
    if ((new + mutexLocked) & mutexLocked == 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 374943) STRING
                     "sync: unlock of unlocked mutex"}));
    }
    if (new &mutexStarving == 0) {
        old = new;
        while (true) {
            if (old >> mutexWaiterShift == 0 ||
                old & (mutexLocked | mutexWoken | mutexStarving) != 0) {
                return;
            }
            new = (old - 1 << mutexWaiterShift) | mutexWoken;
            if (atomic::CompareAndSwapInt32(&this->state, old, new)) {
                runtime_Semrelease(&this->sema, false);
                return;
            }
            old = this->state;
        }
    } else {
        runtime_Semrelease(&this->sema, true);
    }
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
void Once::Do(std::function<void()> f)
{
    moku::defer _defer_;

    if (atomic::LoadUint32(&this->done) == 1) {
        return;
    }
    this->m.Lock();
    _defer_.Push([=]() mutable { this->m.Unlock(); });
    if (this->done == 0) {
        _defer_.Push([=]() mutable { atomic::StoreUint32(&this->done, 1); });
        f();
    }
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
uint32_t fastrand()
{
}
void *poolRaceAddr(moku::interface x)
{
    uint32_t h{0};
    uintptr_t ptr{0};

    ptr = uintptr_t((*std::vector<unsafe::Pointer>)(unsafe::Pointer(&x))[1]);
    h = uint32_t((uint64_t(uint32_t(ptr)) * 0x85ebca6b) >> 16);
    return unsafe::Pointer(&poolRaceHash[h % uint32_t(len(poolRaceHash))]);
}
void Pool::Put(moku::interface x)
{
    poolLocal *l{std::nullptr};

    if (moku::is_nil(x)) {
        return;
    }
    if (race::Enabled) {
        if (fastrand() % 4 == 0) {
            return;
        }
        race::ReleaseMerge(poolRaceAddr(moku::make_iface<moku::interface>(x)));
        race::Disable();
    }
    l = this->pin();
    if (moku::is_nil(l->private)) {
        l->private = x;
        x = nil;
    }
    runtime_procUnpin();
    if (!moku::is_nil(x)) {
        l->Lock();
        l->shared = append(l->shared, x);
        l->Unlock();
    }
    if (race::Enabled) {
        race::Enable();
    }
}
moku::interface Pool::Get()
{
    poolLocal *l{std::nullptr};
    moku::interface x{};

    if (race::Enabled) {
        race::Disable();
    }
    l = this->pin();
    x = l->private;
    l->private = nil;
    runtime_procUnpin();
    if (moku::is_nil(x)) {
        l->Lock();
        last = len(l->shared) - 1;
        if (last >= 0) {
            x = l->shared[last];
            l->shared = moku::slice_expr<moku::slice<moku::interface>>(
                    l->shared, last);
        }
        l->Unlock();
        if (moku::is_nil(x)) {
            x = this->getSlow();
        }
    }
    if (race::Enabled) {
        race::Enable();
        if (!moku::is_nil(x)) {
            race::Acquire(poolRaceAddr(moku::make_iface<moku::interface>(x)));
        }
    }
    if (moku::is_nil(x) && !moku::is_nil(this->New)) {
        x = this->New();
    }
    return x;
}
moku::interface Pool::getSlow()
{
    void *local{std::nullptr};
    int pid{0};
    uintptr_t size{0};
    moku::interface x{};

    size = atomic::LoadUintptr(&this->localSize);
    local = this->local;
    pid = runtime_procPin();
    runtime_procUnpin();
    {
        int i{0};
        for (i = 0; i < int(size); i++) {
            poolLocal *l{std::nullptr};
            int last{0};

            l = indexLocal(local, (pid + i + 1) % int(size));
            l->Lock();
            last = len(l->shared) - 1;
            if (last >= 0) {
                x = l->shared[last];
                l->shared = moku::slice_expr<moku::slice<moku::interface>>(
                        l->shared, last);
                l->Unlock();
                break;
            }
            l->Unlock();
        }
    }
    return x;
}
poolLocal *Pool::pin()
{
    void *l{std::nullptr};
    int pid{0};
    uintptr_t s{0};

    pid = runtime_procPin();
    s = atomic::LoadUintptr(&this->localSize);
    l = this->local;
    if (uintptr_t(pid) < s) {
        return indexLocal(l, pid);
    }
    return this->pinSlow();
}
poolLocal *Pool::pinSlow()
{
    moku::defer _defer_;
    void *l{std::nullptr};
    moku::slice<poolLocal> local{};
    int pid{0};
    uintptr_t s{0};
    int size{0};

    runtime_procUnpin();
    allPoolsMu.Lock();
    _defer_.Push([=]() mutable { allPoolsMu.Unlock(); });
    pid = runtime_procPin();
    s = this->localSize;
    l = this->local;
    if (uintptr_t(pid) < s) {
        return indexLocal(l, pid);
    }
    if (moku::is_nil(this->local)) {
        allPools = append(allPools, this);
    }
    size = runtime::GOMAXPROCS(0);
    local = make(moku::slice<poolLocal>, size);
    atomic::StorePointer(&this->local, unsafe::Pointer(&local[0]));
    atomic::StoreUintptr(&this->localSize, uintptr_t(size));
    return &local[pid];
}
void poolCleanup()
{
    {
        int i{0};
        Pool *p{std::nullptr};
        for (std::tie(i, p) :
             moku::range_key_value<moku::slice<Pool *>>(allPools)) {
            allPools[i] = nil;
            {
                int i{0};
                for (i = 0; i < int(p->localSize); i++) {
                    poolLocal *l{std::nullptr};

                    l = indexLocal(p->local, i);
                    l->private = nil;
                    {
                        int j{0};
                        for (j : moku::range_key<moku::slice<moku::interface>>(
                                     l->shared)) {
                            l->shared[j] = nil;
                        }
                    }
                    l->shared = nil;
                }
            }
            p->local = nil;
            p->localSize = 0;
        }
    }
    allPools = moku::slice<*Pool>{};
}
void init()
{
    runtime_registerPoolCleanup(poolCleanup);
}
poolLocal *indexLocal(void *l, int i)
{
    void *lp{std::nullptr};

    lp = unsafe::Pointer(uintptr_t(l) +
                         uintptr_t(i) * unsafe::Sizeof(poolLocal{}));
    return (*poolLocal)(lp);
}
void runtime_registerPoolCleanup(std::function<void()> cleanup)
{
}
int runtime_procPin()
{
}
void runtime_procUnpin()
{
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
void runtime_Semacquire(uint32_t *s)
{
}
void runtime_SemacquireMutex(uint32_t *s, bool lifo)
{
}
void runtime_Semrelease(uint32_t *s, bool handoff)
{
}
uint32_t runtime_notifyListAdd(notifyList *l)
{
}
void runtime_notifyListWait(notifyList *l, uint32_t t)
{
}
void runtime_notifyListNotifyAll(notifyList *l)
{
}
void runtime_notifyListNotifyOne(notifyList *l)
{
}
void runtime_notifyListCheck(uintptr_t size)
{
}
void init()
{
    notifyList n{};

    runtime_notifyListCheck(unsafe::Sizeof(n));
}
bool runtime_canSpin(int i)
{
}
void runtime_doSpin()
{
}
int64_t runtime_nanotime()
{
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
void RWMutex::RLock()
{
    if (race::Enabled) {
        _ = this->w.state;
        race::Disable();
    }
    if (atomic::AddInt32(&this->readerCount, 1) < 0) {
        runtime_Semacquire(&this->readerSem);
    }
    if (race::Enabled) {
        race::Enable();
        race::Acquire(unsafe::Pointer(&this->readerSem));
    }
}
void RWMutex::RUnlock()
{
    if (race::Enabled) {
        _ = this->w.state;
        race::ReleaseMerge(unsafe::Pointer(&this->writerSem));
        race::Disable();
    }
    {
        int32_t r{0};

        r = atomic::AddInt32(&this->readerCount, -1);

        if (r < 0) {
            if (r + 1 == 0 || r + 1 == -rwmutexMaxReaders) {
                race::Enable();
                throw("sync: RUnlock of unlocked RWMutex");
            }
            if (atomic::AddInt32(&this->readerWait, -1) == 0) {
                runtime_Semrelease(&this->writerSem, false);
            }
        }
    }
    if (race::Enabled) {
        race::Enable();
    }
}
void RWMutex::Lock()
{
    int32_t r{0};

    if (race::Enabled) {
        _ = this->w.state;
        race::Disable();
    }
    this->w.Lock();
    r = atomic::AddInt32(&this->readerCount, -rwmutexMaxReaders) +
        rwmutexMaxReaders;
    if (r != 0 && atomic::AddInt32(&this->readerWait, r) != 0) {
        runtime_Semacquire(&this->writerSem);
    }
    if (race::Enabled) {
        race::Enable();
        race::Acquire(unsafe::Pointer(&this->readerSem));
        race::Acquire(unsafe::Pointer(&this->writerSem));
    }
}
void RWMutex::Unlock()
{
    int32_t r{0};

    if (race::Enabled) {
        _ = this->w.state;
        race::Release(unsafe::Pointer(&this->readerSem));
        race::Release(unsafe::Pointer(&this->writerSem));
        race::Disable();
    }
    r = atomic::AddInt32(&this->readerCount, rwmutexMaxReaders);
    if (r >= rwmutexMaxReaders) {
        race::Enable();
        throw("sync: Unlock of unlocked RWMutex");
    }
    {
        int i{0};
        for (i = 0; i < int(r); i++) {
            runtime_Semrelease(&this->readerSem, false);
        }
    }
    this->w.Unlock();
    if (race::Enabled) {
        race::Enable();
    }
}
Locker RWMutex::RLocker()
{
    return (*rlocker)(this);
}
void rlocker::Lock()
{
    (*RWMutex)(this)->RLock();
}
void rlocker::Unlock()
{
    (*RWMutex)(this)->RUnlock();
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
uint64_t *WaitGroup::state()
{
    if (uintptr_t(unsafe::Pointer(&this->state1)) % 8 == 0) {
        return (*uint64_t)(unsafe::Pointer(&this->state1));
    } else {
        return (*uint64_t)(unsafe::Pointer(&this->state1[4]));
    }
}
void WaitGroup::Add(int delta)
{
    moku::defer _defer_;
    uint64_t state{0};
    uint64_t *statep{std::nullptr};
    int32_t v{0};
    uint32_t w{0};

    statep = this->state();
    if (race::Enabled) {
        _ = *statep;
        if (delta < 0) {
            race::ReleaseMerge(unsafe::Pointer(this));
        }
        race::Disable();
        _defer_.Push([=]() mutable { race::Enable(); });
    }
    state = atomic::AddUint64(statep, uint64_t(delta) << 32);
    v = int32_t(state >> 32);
    w = uint32_t(state);
    if (race::Enabled) {
        if (delta > 0 && v == int32_t(delta)) {
            race::Read(unsafe::Pointer(&this->sema));
        }
    }
    if (v < 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 340907) STRING
                     "sync: negative WaitGroup counter"}));
    }
    if (w != 0 && delta > 0 && v == int32_t(delta)) {
        panic(moku::make_iface<std::string>(&{
                %
                !s(token.Pos = 341001) STRING
                "sync: WaitGroup misuse: Add called concurrently with Wait"}));
    }
    if (v > 0 || w == 0) {
        return;
    }
    if (*statep != state) {
        panic(moku::make_iface<std::string>(&{
                %
                !s(token.Pos = 341418) STRING
                "sync: WaitGroup misuse: Add called concurrently with Wait"}));
    }
    *statep = 0;
    for (; w != 0; w--) {
        runtime_Semrelease(&this->sema, false);
    }
}
void WaitGroup::Done()
{
    this->Add(-1);
}
void WaitGroup::Wait()
{
    uint64_t *statep{std::nullptr};

    statep = this->state();
    if (race::Enabled) {
        _ = *statep;
        race::Disable();
    }
    while (true) {
        uint64_t state{0};
        int32_t v{0};
        uint32_t w{0};

        state = atomic::LoadUint64(statep);
        v = int32_t(state >> 32);
        w = uint32_t(state);
        if (v == 0) {
            if (race::Enabled) {
                race::Enable();
                race::Acquire(unsafe::Pointer(this));
            }
            return;
        }
        if (atomic::CompareAndSwapUint64(statep, state, state + 1)) {
            if (race::Enabled && w == 0) {
                race::Write(unsafe::Pointer(&this->sema));
            }
            runtime_Semacquire(&this->sema);
            if (*statep != 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 342587) STRING
                             "sync: WaitGroup is reused before previous Wait "
                             "has returned"}));
            }
            if (race::Enabled) {
                race::Enable();
                race::Acquire(unsafe::Pointer(this));
            }
            return;
        }
    }
}
void init();
int main()
{
    init();
    expunged = unsafe::Pointer(new (moku::make_iface<moku::interface>(
            &{ % !s(token.Pos = 346697) %
               !s(*ast.FieldList = &{346706 [] 346707}) % !s(bool = false)})));
    _main();
    return 0;
}
