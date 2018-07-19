std::tuple<User *, moku::error> Current()
{
    User u{};

    cache.Do([=]() mutable -> void {
        std::tie(cache.u, cache.err) = current();
    });
    if (!moku::is_nil(cache.err)) {
        return {nil, cache.err};
    }
    u = *cache.u;
    return {&u, nil};
}
std::tuple<User *, moku::error> Lookup(std::string username)
{
    {
        moku::error err{};
        User *u{std::nullptr};

        std::tie(u, err) = Current();

        if (moku::is_nil(err) && u->Username == username) {
            return {u, err};
        }
    }
    return lookupUser(username);
}
std::tuple<User *, moku::error> LookupId(std::string uid)
{
    {
        moku::error err{};
        User *u{std::nullptr};

        std::tie(u, err) = Current();

        if (moku::is_nil(err) && u->Uid == uid) {
            return {u, err};
        }
    }
    return lookupUserId(uid);
}
std::tuple<Group *, moku::error> LookupGroup(std::string name)
{
    return lookupGroup(name);
}
std::tuple<Group *, moku::error> LookupGroupId(std::string gid)
{
    return lookupGroupId(gid);
}
std::tuple<moku::slice<std::string>, moku::error> User::GroupIds()
{
    return listGroups(this);
}
int main()
{
    userImplemented = true;
    groupImplemented = true;
    _cgo_6d86c953a46a_Cfunc_free =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_free);
    _cgo_6d86c953a46a_Cfunc_mygetgrgid_r =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_mygetgrgid_r);
    _cgo_6d86c953a46a_Cfunc_mygetgrnam_r =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_mygetgrnam_r);
    _cgo_6d86c953a46a_Cfunc_mygetgrouplist =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_mygetgrouplist);
    _cgo_6d86c953a46a_Cfunc_mygetpwnam_r =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_mygetpwnam_r);
    _cgo_6d86c953a46a_Cfunc_mygetpwuid_r =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_mygetpwuid_r);
    _cgo_6d86c953a46a_Cfunc_realloc =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_realloc);
    _cgo_6d86c953a46a_Cfunc_sysconf =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc_sysconf);
    _cgo_6d86c953a46a_Cfunc__Cmalloc =
            unsafe::Pointer(&__cgofn__cgo_6d86c953a46a_Cfunc__Cmalloc);
    _main();
    return 0;
}
