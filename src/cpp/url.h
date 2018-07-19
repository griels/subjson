#pragma once
#include "bytes.h"
#include "errors.h"
#include "fmt.h"
#include "sort.h"
#include "strconv.h"
#include "strings.h"
namespace url {

struct Error {
    std::string Op{""};
    std::string URL{""};
    moku::error Err;
    std::string Error();
    bool Timeout();
    bool Temporary();
};
template <>
inline bool moku::is_nil<Error>(const Error &error)
{
    return moku::is_nil<std::string>(error.Op) &&
           moku::is_nil<std::string>(error.URL) &&
           moku::is_nil<moku::error>(error.Err);
}
struct EscapeError : public moku::basic<std::string> {
    EscapeError() : moku::basic<std::string>{""}
    {
    }
};
template <>
inline bool moku::is_nil<EscapeError>(const EscapeError &escapeerror)
{
    return moku::is_nil<std::string>(std::string(escapeerror));
}
inline std::string Error()
{
    EscapeError escapeerror = *this;
    return escapeerror._ErrorByValue();
}
std::string _ErrorByValue();

struct InvalidHostError : public moku::basic<std::string> {
    InvalidHostError() : moku::basic<std::string>{""}
    {
    }
};
template <>
inline bool moku::is_nil<InvalidHostError>(
        const InvalidHostError &invalidhosterror)
{
    return moku::is_nil<std::string>(std::string(invalidhosterror));
}
inline std::string Error()
{
    InvalidHostError invalidhosterror = *this;
    return invalidhosterror._ErrorByValue();
}
std::string _ErrorByValue();
std::tuple<URL *, moku::error> Parse(std::string rawurl);
std::tuple<Values, moku::error> ParseQuery(std::string query);
std::tuple<URL *, moku::error> ParseRequestURI(std::string rawurl);
std::string PathEscape(std::string s);
std::tuple<std::string, moku::error> PathUnescape(std::string s);
std::string QueryEscape(std::string s);
std::tuple<std::string, moku::error> QueryUnescape(std::string s);

struct URL {
    std::string Scheme{""};
    std::string Opaque{""};
    Userinfo *User{std::nullptr};
    std::string Host{""};
    std::string Path{""};
    std::string RawPath{""};
    bool ForceQuery{false};
    std::string RawQuery{""};
    std::string Fragment{""};
    moku::error setPath(std::string p);
    std::string EscapedPath();
    std::string String();
    bool IsAbs();
    std::tuple<URL *, moku::error> Parse(std::string ref);
    URL *ResolveReference(URL *ref);
    Values Query();
    std::string RequestURI();
    std::string Hostname();
    std::string Port();
    std::tuple<moku::slice<uint8_t>, moku::error> MarshalBinary();
    moku::error UnmarshalBinary(moku::slice<uint8_t> text);
};
template <>
inline bool moku::is_nil<URL>(const URL &url)
{
    return moku::is_nil<std::string>(url.Scheme) &&
           moku::is_nil<std::string>(url.Opaque) &&
           moku::is_nil<Userinfo *>(url.User) &&
           moku::is_nil<std::string>(url.Host) &&
           moku::is_nil<std::string>(url.Path) &&
           moku::is_nil<std::string>(url.RawPath) &&
           moku::is_nil<bool>(url.ForceQuery) &&
           moku::is_nil<std::string>(url.RawQuery) &&
           moku::is_nil<std::string>(url.Fragment);
}
Userinfo *User(std::string username);
Userinfo *UserPassword(std::string username, std::string password);

struct Userinfo {
    std::string username{""};
    std::string password{""};
    bool passwordSet{false};
    std::string Username();
    std::tuple<std::string, bool> Password();
    std::string String();
};
template <>
inline bool moku::is_nil<Userinfo>(const Userinfo &userinfo)
{
    return moku::is_nil<std::string>(userinfo.username) &&
           moku::is_nil<std::string>(userinfo.password) &&
           moku::is_nil<bool>(userinfo.passwordSet);
}
} // namespace url
