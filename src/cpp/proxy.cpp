std::tuple<Conn, moku::error> direct::_DialByValue(std::string network,
                                                   std::string addr)
{
    return net::Dial(network, addr);
}
int main()
{
    Direct = direct{};
    socks5Errors = moku::slice<std::string>{"",
                                            "general failure",
                                            "connection forbidden",
                                            "network unreachable",
                                            "host unreachable",
                                            "connection refused",
                                            "TTL expired",
                                            "command not supported",
                                            "address type not supported"};
    _main();
    return 0;
}
PerHost *NewPerHost(Dialer defaultDialer, Dialer bypass)
{
    return &PerHost{{def, defaultDialer}, {bypass, bypass}};
}
std::tuple<Conn, moku::error> PerHost::Dial(std::string network,
                                            std::string addr)
{
    Conn c{};
    moku::error err{};
    std::string host{""};

    std::tie(host, _, err) = net::SplitHostPort(addr);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    return this->dialerForRequest(host).Dial(network, addr);
}
Dialer PerHost::dialerForRequest(std::string host)
{
    {
        IP ip{};

        ip = net::ParseIP(host);

        if (!moku::is_nil(ip)) {
            {
                IPNet *net{std::nullptr};
                for (net : moku::range_value<moku::slice<IPNet *>>(
                             this->bypassNetworks)) {
                    if (net::Contains(ip)) {
                        return this->bypass;
                    }
                }
            }
            {
                IP bypassIP{};
                for (bypassIP :
                     moku::range_value<moku::slice<IP>>(this->bypassIPs)) {
                    if (bypassIP.Equal(ip)) {
                        return this->bypass;
                    }
                }
            }
            return this->def;
        }
    }
    {
        std::string zone{""};
        for (zone :
             moku::range_value<moku::slice<std::string>>(this->bypassZones)) {
            if (strings::HasSuffix(host, zone)) {
                return this->bypass;
            }
            if (host == moku::slice_expr<std::string>(zone, 1)) {
                return this->bypass;
            }
        }
    }
    {
        std::string bypassHost{""};
        for (bypassHost :
             moku::range_value<moku::slice<std::string>>(this->bypassHosts)) {
            if (bypassHost == host) {
                return this->bypass;
            }
        }
    }
    return this->def;
}
void PerHost::AddFromString(std::string s)
{
    moku::slice<std::string> hosts{};

    hosts = strings::Split(s, ",");
    {
        std::string host{""};
        for (host : moku::range_value<moku::slice<std::string>>(hosts)) {
            host = strings::TrimSpace(host);
            if (len(host) == 0) {
                continue;
            }
            if (strings::Contains(host, "/")) {
                {
                    moku::error err{};
                    IPNet *net{std::nullptr};

                    std::tie(_, net, err) = net::ParseCIDR(host);

                    if (moku::is_nil(err)) {
                        this->AddNetwork(net);
                    }
                }
                continue;
            }
            {
                IP ip{};

                ip = net::ParseIP(host);

                if (!moku::is_nil(ip)) {
                    this->AddIP(ip);
                    continue;
                }
            }
            if (strings::HasPrefix(host, "*.")) {
                this->AddZone(moku::slice_expr<std::string>(host, 1));
                continue;
            }
            this->AddHost(host);
        }
    }
}
void PerHost::AddIP(IP ip)
{
    this->bypassIPs = append(this->bypassIPs, ip);
}
void PerHost::AddNetwork(IPNet *net)
{
    this->bypassNetworks = append(this->bypassNetworks, net);
}
void PerHost::AddZone(std::string zone)
{
    if (strings::HasSuffix(zone, ".")) {
        zone = moku::slice_expr<std::string>(zone, len(zone) - 1);
    }
    if (!strings::HasPrefix(zone, ".")) {
        zone = "." + zone;
    }
    this->bypassZones = append(this->bypassZones, zone);
}
void PerHost::AddHost(std::string host)
{
    if (strings::HasSuffix(host, ".")) {
        host = moku::slice_expr<std::string>(host, len(host) - 1);
    }
    this->bypassHosts = append(this->bypassHosts, host);
}
int main()
{
    Direct = direct{};
    socks5Errors = moku::slice<std::string>{"",
                                            "general failure",
                                            "connection forbidden",
                                            "network unreachable",
                                            "host unreachable",
                                            "connection refused",
                                            "TTL expired",
                                            "command not supported",
                                            "address type not supported"};
    _main();
    return 0;
}
Dialer FromEnvironment()
{
    std::string allProxy{""};
    moku::error err{};
    std::string noProxy{""};
    PerHost *perHost{std::nullptr};
    Dialer proxy{};
    URL *proxyURL{std::nullptr};

    allProxy = os::Getenv("all_proxy");
    if (len(allProxy) == 0) {
        return Direct;
    }
    std::tie(proxyURL, err) = url::Parse(allProxy);
    if (!moku::is_nil(err)) {
        return Direct;
    }
    std::tie(proxy, err) = FromURL(proxyURL, Direct);
    if (!moku::is_nil(err)) {
        return Direct;
    }
    noProxy = os::Getenv("no_proxy");
    if (len(noProxy) == 0) {
        return proxy;
    }
    perHost = NewPerHost(proxy, Direct);
    perHost->AddFromString(noProxy);
    return perHost;
}
void RegisterDialerType(std::string scheme,
                        std::function<Dialer, moku::error(URL *, Dialer)> f)
{
}
