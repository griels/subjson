#pragma once
#include "cgo.h"
#include "fmt.h"
#include "strconv.h"
#include "strings.h"
#include "sync.h"
#include "syscall.h"
#include "unsafe.h"
namespace user {
std::tuple<User *, moku::error> Current();

struct Group {
    std::string Gid{""};
    std::string Name{""};
};
template <>
inline bool moku::is_nil<Group>(const Group &group)
{
    return moku::is_nil<std::string>(group.Gid) &&
           moku::is_nil<std::string>(group.Name);
}
std::tuple<User *, moku::error> Lookup(std::string username);
std::tuple<Group *, moku::error> LookupGroup(std::string name);
std::tuple<Group *, moku::error> LookupGroupId(std::string gid);
std::tuple<User *, moku::error> LookupId(std::string uid);

struct UnknownGroupError : public moku::basic<std::string> {
    UnknownGroupError() : moku::basic<std::string>{""}
    {
    }
};
template <>
inline bool moku::is_nil<UnknownGroupError>(
        const UnknownGroupError &unknowngrouperror)
{
    return moku::is_nil<std::string>(std::string(unknowngrouperror));
}
inline std::string Error()
{
    UnknownGroupError unknowngrouperror = *this;
    return unknowngrouperror._ErrorByValue();
}
std::string _ErrorByValue();

struct UnknownGroupIdError : public moku::basic<std::string> {
    UnknownGroupIdError() : moku::basic<std::string>{""}
    {
    }
};
template <>
inline bool moku::is_nil<UnknownGroupIdError>(
        const UnknownGroupIdError &unknowngroupiderror)
{
    return moku::is_nil<std::string>(std::string(unknowngroupiderror));
}
inline std::string Error()
{
    UnknownGroupIdError unknowngroupiderror = *this;
    return unknowngroupiderror._ErrorByValue();
}
std::string _ErrorByValue();

struct UnknownUserError : public moku::basic<std::string> {
    UnknownUserError() : moku::basic<std::string>{""}
    {
    }
};
template <>
inline bool moku::is_nil<UnknownUserError>(
        const UnknownUserError &unknownusererror)
{
    return moku::is_nil<std::string>(std::string(unknownusererror));
}
inline std::string Error()
{
    UnknownUserError unknownusererror = *this;
    return unknownusererror._ErrorByValue();
}
std::string _ErrorByValue();

struct UnknownUserIdError : public moku::basic<int> {
    UnknownUserIdError() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<UnknownUserIdError>(
        const UnknownUserIdError &unknownuseriderror)
{
    return moku::is_nil<int>(int(unknownuseriderror));
}
inline std::string Error()
{
    UnknownUserIdError unknownuseriderror = *this;
    return unknownuseriderror._ErrorByValue();
}
std::string _ErrorByValue();

struct User {
    std::string Uid{""};
    std::string Gid{""};
    std::string Username{""};
    std::string Name{""};
    std::string HomeDir{""};
    std::tuple<moku::slice<std::string>, moku::error> GroupIds();
};
template <>
inline bool moku::is_nil<User>(const User &user)
{
    return moku::is_nil<std::string>(user.Uid) &&
           moku::is_nil<std::string>(user.Gid) &&
           moku::is_nil<std::string>(user.Username) &&
           moku::is_nil<std::string>(user.Name) &&
           moku::is_nil<std::string>(user.HomeDir);
}
_Ctype_char *_Cfunc_CString(std::string s);
std::string _Cfunc_GoString(_Ctype_char *p);
void *_Cfunc__CMalloc(_Ctype_size_t n);
_Ctype_void _Cfunc_free(void *p0);
_Ctype_int _Cfunc_mygetgrgid_r(_Ctype_int p0,
                               _Ctype_struct_group *p1,
                               _Ctype_char *p2,
                               _Ctype_size_t p3,
                               _Ctype_struct_group **p4);
_Ctype_int _Cfunc_mygetgrnam_r(_Ctype_char *p0,
                               _Ctype_struct_group *p1,
                               _Ctype_char *p2,
                               _Ctype_size_t p3,
                               _Ctype_struct_group **p4);
_Ctype_int _Cfunc_mygetgrouplist(_Ctype_char *p0,
                                 _Ctype_gid_t p1,
                                 _Ctype_gid_t *p2,
                                 _Ctype_int *p3);
_Ctype_int _Cfunc_mygetpwnam_r(_Ctype_char *p0,
                               _Ctype_struct_passwd *p1,
                               _Ctype_char *p2,
                               _Ctype_size_t p3,
                               _Ctype_struct_passwd **p4);
_Ctype_int _Cfunc_mygetpwuid_r(_Ctype_int p0,
                               _Ctype_struct_passwd *p1,
                               _Ctype_char *p2,
                               _Ctype_size_t p3,
                               _Ctype_struct_passwd **p4);
void *_Cfunc_realloc(void *p0, _Ctype_size_t p1);
_Ctype_long _Cfunc_sysconf(_Ctype_int p0);
static bool _Cgo_always_false;
void *_Cgo_ptr(void *ptr);
void _Cgo_use(moku::interface);
static constexpr int _Ciconst__SC_GETGR_R_SIZE_MAX{70};
static constexpr int _Ciconst__SC_GETPW_R_SIZE_MAX{71};

struct _Ctype___darwin_gid_t : public moku::basic<uint32_t> {
    _Ctype___darwin_gid_t() : moku::basic<uint32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype___darwin_gid_t>(
        const _Ctype___darwin_gid_t &_ctype___darwin_gid_t)
{
    return moku::is_nil<uint32_t>(uint32_t(_ctype___darwin_gid_t));
}
struct _Ctype___darwin_size_t : public moku::basic<uint64_t> {
    _Ctype___darwin_size_t() : moku::basic<uint64_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype___darwin_size_t>(
        const _Ctype___darwin_size_t &_ctype___darwin_size_t)
{
    return moku::is_nil<uint64_t>(uint64_t(_ctype___darwin_size_t));
}
struct _Ctype___darwin_time_t : public moku::basic<int64_t> {
    _Ctype___darwin_time_t() : moku::basic<int64_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype___darwin_time_t>(
        const _Ctype___darwin_time_t &_ctype___darwin_time_t)
{
    return moku::is_nil<int64_t>(int64_t(_ctype___darwin_time_t));
}
struct _Ctype___darwin_uid_t : public moku::basic<uint32_t> {
    _Ctype___darwin_uid_t() : moku::basic<uint32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype___darwin_uid_t>(
        const _Ctype___darwin_uid_t &_ctype___darwin_uid_t)
{
    return moku::is_nil<uint32_t>(uint32_t(_ctype___darwin_uid_t));
}
struct _Ctype___uint32_t : public moku::basic<uint32_t> {
    _Ctype___uint32_t() : moku::basic<uint32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype___uint32_t>(
        const _Ctype___uint32_t &_ctype___uint32_t)
{
    return moku::is_nil<uint32_t>(uint32_t(_ctype___uint32_t));
}
struct _Ctype_char : public moku::basic<int8_t> {
    _Ctype_char() : moku::basic<int8_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_char>(const _Ctype_char &_ctype_char)
{
    return moku::is_nil<int8_t>(int8_t(_ctype_char));
}
struct _Ctype_gid_t : public moku::basic<uint32_t> {
    _Ctype_gid_t() : moku::basic<uint32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_gid_t>(const _Ctype_gid_t &_ctype_gid_t)
{
    return moku::is_nil<uint32_t>(uint32_t(_ctype_gid_t));
}
struct _Ctype_int : public moku::basic<int32_t> {
    _Ctype_int() : moku::basic<int32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_int>(const _Ctype_int &_ctype_int)
{
    return moku::is_nil<int32_t>(int32_t(_ctype_int));
}
struct _Ctype_long : public moku::basic<int64_t> {
    _Ctype_long() : moku::basic<int64_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_long>(const _Ctype_long &_ctype_long)
{
    return moku::is_nil<int64_t>(int64_t(_ctype_long));
}
struct _Ctype_size_t : public moku::basic<uint64_t> {
    _Ctype_size_t() : moku::basic<uint64_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_size_t>(const _Ctype_size_t &_ctype_size_t)
{
    return moku::is_nil<uint64_t>(uint64_t(_ctype_size_t));
}
struct _Ctype_struct_group {
    _Ctype_char *gr_name{std::nullptr};
    _Ctype_char *gr_passwd{std::nullptr};
    _Ctype_gid_t gr_gid;
    std::vector<uint8_t> _;
    _Ctype_char **gr_mem{std::nullptr};
};
template <>
inline bool moku::is_nil<_Ctype_struct_group>(
        const _Ctype_struct_group &_ctype_struct_group)
{
    return moku::is_nil<_Ctype_char *>(_ctype_struct_group.gr_name) &&
           moku::is_nil<_Ctype_char *>(_ctype_struct_group.gr_passwd) &&
           moku::is_nil<_Ctype_gid_t>(_ctype_struct_group.gr_gid) &&
           moku::is_nil<std::vector<uint8_t>>(_ctype_struct_group._) &&
           moku::is_nil<_Ctype_char **>(_ctype_struct_group.gr_mem);
}
struct _Ctype_struct_passwd {
    _Ctype_char *pw_name{std::nullptr};
    _Ctype_char *pw_passwd{std::nullptr};
    _Ctype_uid_t pw_uid;
    _Ctype_gid_t pw_gid;
    _Ctype___darwin_time_t pw_change;
    _Ctype_char *pw_class{std::nullptr};
    _Ctype_char *pw_gecos{std::nullptr};
    _Ctype_char *pw_dir{std::nullptr};
    _Ctype_char *pw_shell{std::nullptr};
    _Ctype___darwin_time_t pw_expire;
};
template <>
inline bool moku::is_nil<_Ctype_struct_passwd>(
        const _Ctype_struct_passwd &_ctype_struct_passwd)
{
    return moku::is_nil<_Ctype_char *>(_ctype_struct_passwd.pw_name) &&
           moku::is_nil<_Ctype_char *>(_ctype_struct_passwd.pw_passwd) &&
           moku::is_nil<_Ctype_uid_t>(_ctype_struct_passwd.pw_uid) &&
           moku::is_nil<_Ctype_gid_t>(_ctype_struct_passwd.pw_gid) &&
           moku::is_nil<_Ctype___darwin_time_t>(
                   _ctype_struct_passwd.pw_change) &&
           moku::is_nil<_Ctype_char *>(_ctype_struct_passwd.pw_class) &&
           moku::is_nil<_Ctype_char *>(_ctype_struct_passwd.pw_gecos) &&
           moku::is_nil<_Ctype_char *>(_ctype_struct_passwd.pw_dir) &&
           moku::is_nil<_Ctype_char *>(_ctype_struct_passwd.pw_shell) &&
           moku::is_nil<_Ctype___darwin_time_t>(_ctype_struct_passwd.pw_expire);
}
struct _Ctype_uid_t : public moku::basic<uint32_t> {
    _Ctype_uid_t() : moku::basic<uint32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_uid_t>(const _Ctype_uid_t &_ctype_uid_t)
{
    return moku::is_nil<uint32_t>(uint32_t(_ctype_uid_t));
}
struct _Ctype_uint : public moku::basic<uint32_t> {
    _Ctype_uint() : moku::basic<uint32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_uint>(const _Ctype_uint &_ctype_uint)
{
    return moku::is_nil<uint32_t>(uint32_t(_ctype_uint));
}
struct _Ctype_ulong : public moku::basic<uint64_t> {
    _Ctype_ulong() : moku::basic<uint64_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<_Ctype_ulong>(const _Ctype_ulong &_ctype_ulong)
{
    return moku::is_nil<uint64_t>(uint64_t(_ctype_ulong));
}
} // namespace user
