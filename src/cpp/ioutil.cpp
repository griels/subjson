std::tuple<moku::slice<uint8_t>, moku::error> readAll(Reader r,
                                                      int64_t capacity)
{
    moku::defer _defer_;
    moku::slice<uint8_t> b{};
    Buffer *buf{std::nullptr};
    moku::error err{};

    buf = bytes::NewBuffer(make(moku::slice<byte>, 0, capacity));
    _defer_.Push([=]() mutable {
        [=]() mutable -> void {
            e = recover();
            if (moku::is_nil(e)) {
                return;
            }
            {
                bool ok{false};
                moku::error panicErr{};

                std::tie(panicErr, ok) = moku::try_assert<error>(e);

                if (ok && panicErr == bytes::ErrTooLarge) {
                    err = panicErr;
                } else {
                    panic(moku::make_iface<moku::interface>(e));
                }
            }
        }();
    });
    std::tie(_, err) = buf->ReadFrom(r);
    return {buf->Bytes(), err};
}
std::tuple<moku::slice<uint8_t>, moku::error> ReadAll(Reader r)
{
    return readAll(r, bytes::MinRead);
}
std::tuple<moku::slice<uint8_t>, moku::error> ReadFile(std::string filename)
{
    moku::defer _defer_;
    moku::error err{};
    File *f{std::nullptr};
    int64_t n{0};

    std::tie(f, err) = os::Open(filename);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    _defer_.Push([=]() mutable { f->Close(); });
    {
        moku::error err{};
        FileInfo fi{};

        std::tie(fi, err) = f->Stat();

        if (moku::is_nil(err)) {
            {
                int64_t size{0};

                size = fi.Size();

                if (size < 1e9) {
                    n = size;
                }
            }
        }
    }
    return readAll(f, n + bytes::MinRead);
}
moku::error WriteFile(std::string filename,
                      moku::slice<uint8_t> data,
                      FileMode perm)
{
    moku::error err{};
    File *f{std::nullptr};
    int n{0};

    std::tie(f, err) = os::OpenFile(
            filename, os::O_WRONLY | os::O_CREATE | os::O_TRUNC, perm);
    if (!moku::is_nil(err)) {
        return err;
    }
    std::tie(n, err) = f->Write(data);
    if (moku::is_nil(err) && n < len(data)) {
        err = io::ErrShortWrite;
    }
    {
        moku::error err1{};

        err1 = f->Close();

        if (moku::is_nil(err)) {
            err = err1;
        }
    }
    return err;
}
std::tuple<moku::slice<FileInfo>, moku::error> ReadDir(std::string dirname)
{
    moku::error err{};
    File *f{std::nullptr};
    moku::slice<FileInfo> list{};

    std::tie(f, err) = os::Open(dirname);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    std::tie(list, err) = f->Readdir(-1);
    f->Close();
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    sort::Slice(moku::make_iface<moku::slice<FileInfo>>(list),
                [=](int i, int j) mutable -> bool {
                    return list[i].Name() < list[j].Name();
                });
    return {list, nil};
}
moku::error nopCloser::_CloseByValue()
{
    return nil;
}
ReadCloser NopCloser(Reader r)
{
    return nopCloser{r};
}
std::tuple<int, moku::error> devNull::_WriteByValue(moku::slice<uint8_t> p)
{
    return {len(p), nil};
}
std::tuple<int, moku::error> devNull::_WriteStringByValue(std::string s)
{
    return {len(s), nil};
}
std::tuple<int64_t, moku::error> devNull::_ReadFromByValue(Reader r)
{
    moku::slice<uint8_t> *bufp{std::nullptr};
    moku::error err{};
    int64_t n{0};
    int readSize{0};

    bufp = moku::type_assert<*moku::slice<byte>>(blackHolePool.Get());
    readSize = 0;
    while (true) {
        std::tie(readSize, err) = r.Read(*bufp);
        n += int64_t(readSize);
        if (!moku::is_nil(err)) {
            blackHolePool.Put(moku::make_iface<moku::slice<uint8_t> *>(bufp));
            if (err == io::EOF) {
                return {n, nil};
            }
            return;
        }
    }
}
int main()
{
    _ = devNull(0);
    blackHolePool = sync::Pool{{New, [=]() mutable -> moku::interface {
                                    b = make(moku::slice<byte>, 8192);
                                    return &b;
                                }}};
    Discard = devNull(0);
    _main();
    return 0;
}
uint32_t reseed()
{
    return uint32_t(time::Now().UnixNano() + int64_t(os::Getpid()));
}
std::string nextSuffix()
{
    uint32_t r{0};

    randmu.Lock();
    r = rand;
    if (r == 0) {
        r = reseed();
    }
    r = r * 1664525 + 1013904223;
    rand = r;
    randmu.Unlock();
    return moku::slice_expr<std::string>(strconv::Itoa(int(1e9 + r % 1e9)), 1);
}
std::tuple<File *, moku::error> TempFile(std::string dir, std::string prefix)
{
    moku::error err{};
    File *f{std::nullptr};
    int nconflict{0};

    if (dir == "") {
        dir = os::TempDir();
    }
    nconflict = 0;
    {
        int i{0};
        for (i = 0; i < 10000; i++) {
            std::string name{""};

            name = filepath::Join(dir, prefix + nextSuffix());
            std::tie(f, err) = os::OpenFile(
                    name, os::O_RDWR | os::O_CREATE | os::O_EXCL, 0600);
            if (os::IsExist(err)) {
                {
                    nconflict++;

                    if (nconflict > 10) {
                        randmu.Lock();
                        rand = reseed();
                        randmu.Unlock();
                    }
                }
                continue;
            }
            break;
        }
    }
    return;
}
std::tuple<std::string, moku::error> TempDir(std::string dir,
                                             std::string prefix)
{
    moku::error err{};
    std::string name{""};
    int nconflict{0};

    if (dir == "") {
        dir = os::TempDir();
    }
    nconflict = 0;
    {
        int i{0};
        for (i = 0; i < 10000; i++) {
            std::string try {
                ""
            };

            try
                = filepath::Join(dir, prefix + nextSuffix());
            err = os::Mkdir(try, 0700);
            if (os::IsExist(err)) {
                {
                    nconflict++;

                    if (nconflict > 10) {
                        randmu.Lock();
                        rand = reseed();
                        randmu.Unlock();
                    }
                }
                continue;
            }
            if (os::IsNotExist(err)) {
                {
                    moku::error err{};

                    std::tie(_, err) = os::Stat(dir);

                    if (os::IsNotExist(err)) {
                        return {"", err};
                    }
                }
            }
            if (moku::is_nil(err)) {
                name = try
                    ;
            }
            break;
        }
    }
    return;
}
int main()
{
    _ = devNull(0);
    blackHolePool = sync::Pool{{New, [=]() mutable -> moku::interface {
                                    b = make(moku::slice<byte>, 8192);
                                    return &b;
                                }}};
    Discard = devNull(0);
    _main();
    return 0;
}
