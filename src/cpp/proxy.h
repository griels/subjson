#pragma once
#include "errors.h"
#include "io.h"
#include "net.h"
#include "os.h"
#include "strconv.h"
#include "strings.h"
#include "url.h"
namespace proxy {

struct Auth {
    std::string User{""};
    std::string Password{""};
};
template <>
inline bool moku::is_nil<Auth>(const Auth &auth)
{
    return moku::is_nil<std::string>(auth.User) &&
           moku::is_nil<std::string>(auth.Password);
}
struct Dialer {
    virtual std::tuple<Conn, moku::error> Dial(std::string network,
                                               std::string addr) = 0;
};
template <>
inline direct *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<direct>(iface);
}
direct Direct;
Dialer FromEnvironment();
std::tuple<Dialer, moku::error> FromURL(URL *u, Dialer forward);
PerHost *NewPerHost(Dialer defaultDialer, Dialer bypass);

struct PerHost {
    Dialer def;
    Dialer bypass;
    moku::slice<IPNet *> bypassNetworks;
    moku::slice<IP> bypassIPs;
    moku::slice<std::string> bypassZones;
    moku::slice<std::string> bypassHosts;
    std::tuple<Conn, moku::error> Dial(std::string network, std::string addr);
    Dialer dialerForRequest(std::string host);
    void AddFromString(std::string s);
    void AddIP(IP ip);
    void AddNetwork(IPNet *net);
    void AddZone(std::string zone);
    void AddHost(std::string host);
};
template <>
inline bool moku::is_nil<PerHost>(const PerHost &perhost)
{
    return moku::is_nil<Dialer>(perhost.def) &&
           moku::is_nil<Dialer>(perhost.bypass) &&
           moku::is_nil<moku::slice<IPNet *>>(perhost.bypassNetworks) &&
           moku::is_nil<moku::slice<IP>>(perhost.bypassIPs) &&
           moku::is_nil<moku::slice<std::string>>(perhost.bypassZones) &&
           moku::is_nil<moku::slice<std::string>>(perhost.bypassHosts);
}
void RegisterDialerType(std::string scheme,
                        std::function<Dialer, moku::error(URL *, Dialer)> f);
std::tuple<Dialer, moku::error> SOCKS5(std::string network,
                                       std::string addr,
                                       Auth *auth,
                                       Dialer forward);

struct direct : public Dialer {
    inline virtual std::tuple<Conn, moku::error> Dial(std::string network,
                                                      std::string addr) override
    {
        direct direct = *this;
        return direct._DialByValue(std::string network, std::string addr);
    }
    std::tuple<Conn, moku::error> _DialByValue(std::string network,
                                               std::string addr);
};
template <>
inline bool moku::is_nil<direct>(const direct &direct)
{
    return;
}
template <>
direct *moku::try_assert(const moku::interface &iface)
{
    if (Dialer *dialer = moku::type_registry::try_assert<Dialer>(iface))
        return dialer;
    return std::nullptr;
}
static std::map<std::string, std::function<Dialer, moku::error(URL *, Dialer)>>
        proxySchemes;

struct socks5 {
    std::string user{""};
    std::string password{""};
    std::string network{""};
    std::string addr{""};
    Dialer forward;
    std::tuple<Conn, moku::error> Dial(std::string network, std::string addr);
    moku::error connect(Conn conn, std::string target);
};
template <>
inline bool moku::is_nil<socks5>(const socks5 &socks5)
{
    return moku::is_nil<std::string>(socks5.user) &&
           moku::is_nil<std::string>(socks5.password) &&
           moku::is_nil<std::string>(socks5.network) &&
           moku::is_nil<std::string>(socks5.addr) &&
           moku::is_nil<Dialer>(socks5.forward);
}
static constexpr int socks5AuthNone{0};
static constexpr int socks5AuthPassword{2};
static constexpr int socks5Connect{1};
static constexpr int socks5Domain{3};
static moku::slice<std::string> socks5Errors;
static constexpr int socks5IP4{1};
static constexpr int socks5IP6{4};
static constexpr int socks5Version{5};
} // namespace proxy
