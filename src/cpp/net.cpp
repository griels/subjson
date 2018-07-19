void sortByRFC6724(moku::slice<IPAddr> addrs)
{
    if (len(addrs) < 2) {
        return;
    }
    sortByRFC6724withSrcs(addrs, srcAddrs(addrs));
}
void sortByRFC6724withSrcs(moku::slice<IPAddr> addrs, moku::slice<IP> srcs)
{
    moku::slice<ipAttr> addrAttr{};
    moku::slice<ipAttr> srcAttr{};

    if (len(addrs) != len(srcs)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 4045986) STRING "internal error"}));
    }
    addrAttr = make(moku::slice<ipAttr>, len(addrs));
    srcAttr = make(moku::slice<ipAttr>, len(srcs));
    {
        int i{0};
        IPAddr v{};
        for (std::tie(i, v) :
             moku::range_key_value<moku::slice<IPAddr>>(addrs)) {
            addrAttr[i] = ipAttrOf(v.IP);
            srcAttr[i] = ipAttrOf(srcs[i]);
        }
    }
    sort::Stable(&byRFC6724{{addrs, addrs},
                            {addrAttr, addrAttr},
                            {srcs, srcs},
                            {srcAttr, srcAttr}});
}
moku::slice<IP> srcAddrs(moku::slice<IPAddr> addrs)
{
    UDPAddr dst{};
    moku::slice<IP> srcs{};

    srcs = make(moku::slice<IP>, len(addrs));
    dst = UDPAddr{Port : 9};
    {
        int i{0};
        for (i : moku::range_key<moku::slice<IPAddr>>(addrs)) {
            dst.IP = addrs[i].IP;
            dst.Zone = addrs[i].Zone;
            std::tie(c, err) = DialUDP("udp", nil, &dst);
            if (moku::is_nil(err)) {
                {
                    bool ok{false};
                    UDPAddr *src{std::nullptr};

                    std::tie(src, ok) =
                            moku::try_assert<*UDPAddr>(c->LocalAddr());

                    if (ok) {
                        srcs[i] = src->IP;
                    }
                }
                c->Close();
            }
        }
    }
    return srcs;
}
ipAttr ipAttrOf(IP ip)
{
    policyTableEntry match{};

    if (moku::is_nil(ip)) {
        return ipAttr{};
    }
    match = rfc6724policyTable.Classify(ip);
    return ipAttr{{Scope, classifyScope(ip)},
                  {Precedence, match.Precedence},
                  {Label, match.Label}};
}
int byRFC6724::Len()
{
    return len(this->addrs);
}
void byRFC6724::Swap(int i, int j)
{
    std::tie(this->addrs[i], this->addrs[j]) =
            std::tuple<IPAddr, IPAddr>(this->addrs[j], this->addrs[i]);
    std::tie(this->srcs[i], this->srcs[j]) =
            std::tuple<IP, IP>(this->srcs[j], this->srcs[i]);
    std::tie(this->addrAttr[i], this->addrAttr[j]) =
            std::tuple<ipAttr, ipAttr>(this->addrAttr[j], this->addrAttr[i]);
    std::tie(this->srcAttr[i], this->srcAttr[j]) =
            std::tuple<ipAttr, ipAttr>(this->srcAttr[j], this->srcAttr[i]);
}
bool byRFC6724::Less(int i, int j)
{
    IP DA{};
    IP DB{};
    IP SourceDA{};
    IP SourceDB{};
    ipAttr *attrDA{std::nullptr};
    ipAttr *attrDB{std::nullptr};
    ipAttr *attrSourceDA{std::nullptr};
    ipAttr *attrSourceDB{std::nullptr};
    constexpr bool preferDA{true};
    constexpr bool preferDB{false};

    DA = this->addrs[i].IP;
    DB = this->addrs[j].IP;
    SourceDA = this->srcs[i];
    SourceDB = this->srcs[j];
    attrDA = &this->addrAttr[i];
    attrDB = &this->addrAttr[j];
    attrSourceDA = &this->srcAttr[i];
    attrSourceDB = &this->srcAttr[j];
    if (moku::is_nil(SourceDA) && moku::is_nil(SourceDB)) {
        return false;
    }
    if (moku::is_nil(SourceDB)) {
        return preferDA;
    }
    if (moku::is_nil(SourceDA)) {
        return preferDB;
    }
    if (attrDA->Scope == attrSourceDA->Scope &&
        attrDB->Scope != attrSourceDB->Scope) {
        return preferDA;
    }
    if (attrDA->Scope != attrSourceDA->Scope &&
        attrDB->Scope == attrSourceDB->Scope) {
        return preferDB;
    }
    if (attrSourceDA->Label == attrDA->Label &&
        attrSourceDB->Label != attrDB->Label) {
        return preferDA;
    }
    if (attrSourceDA->Label != attrDA->Label &&
        attrSourceDB->Label == attrDB->Label) {
        return preferDB;
    }
    if (attrDA->Precedence > attrDB->Precedence) {
        return preferDA;
    }
    if (attrDA->Precedence < attrDB->Precedence) {
        return preferDB;
    }
    if (attrDA->Scope < attrDB->Scope) {
        return preferDA;
    }
    if (attrDA->Scope > attrDB->Scope) {
        return preferDB;
    }
    if (moku::is_nil(DA.To4()) && moku::is_nil(DB.To4())) {
        commonA = commonPrefixLen(SourceDA, DA);
        commonB = commonPrefixLen(SourceDB, DB);
        if (commonA > commonB) {
            return preferDA;
        }
        if (commonA < commonB) {
            return preferDB;
        }
    }
    return false;
}
void init()
{
    sort::Sort(sort::Reverse(byMaskLength(rfc6724policyTable)));
}
int byMaskLength::_LenByValue()
{
    return len(this);
}
void byMaskLength::_SwapByValue(int i, int j)
{
    std::tie(this[i], this[j]) =
            std::tuple<policyTableEntry, policyTableEntry>(this[j], this[i]);
}
bool byMaskLength::_LessByValue(int i, int j)
{
    int isize{0};
    int jsize{0};

    std::tie(isize, _) = this[i].Prefix->Mask.Size();
    std::tie(jsize, _) = this[j].Prefix->Mask.Size();
    return isize < jsize;
}
IPNet *mustCIDR(std::string s)
{
    moku::error err{};
    IP ip{};
    IPNet *ipNet{std::nullptr};

    std::tie(ip, ipNet, err) = ParseCIDR(s);
    if (!moku::is_nil(err)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(*ast.SelectorExpr = &{0xc424ba2da0 0xc424ba2dc0}) %
                   !s(token.Pos = 4053124)[] % !s(token.Pos = 0) %
                   !s(token.Pos = 4053125)}));
    }
    if (len(ip) != IPv6len) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 4053164) STRING "unexpected IP length"}));
    }
    return ipNet;
}
policyTableEntry policyTable::_ClassifyByValue(IP ip)
{
    {
        policyTableEntry ent{};
        for (ent : moku::range_value<policyTable>(this)) {
            if (ent.Prefix->Contains(ip)) {
                return ent;
            }
        }
    }
    return policyTableEntry{};
}
scope classifyScope(IP ip)
{
    bool ipv6{false};

    if (ip.IsLoopback() || ip.IsLinkLocalUnicast()) {
        return scopeLinkLocal;
    }
    ipv6 = len(ip) == IPv6len && moku::is_nil(ip.To4());
    if (ipv6 && ip.IsMulticast()) {
        return scope(ip[1] & 0xf);
    }
    if (ipv6 && ip[0] == 0xfe && ip[1] & 0xc0 == 0xc0) {
        return scopeSiteLocal;
    }
    return scopeGlobal;
}
int commonPrefixLen(IP a, IP b)
{
    int cpl{0};

    {
        IP a4{};

        a4 = a.To4();

        if (!moku::is_nil(a4)) {
            a = a4;
        }
    }
    {
        IP b4{};

        b4 = b.To4();

        if (!moku::is_nil(b4)) {
            b = b4;
        }
    }
    if (len(a) != len(b)) {
        return 0;
    }
    if (len(a) > 8) {
        a = moku::slice_expr<IP>(a, 8);
        b = moku::slice_expr<IP>(b, 8);
    }
    while (len(a) > 0) {
        uint8_t ab{0};
        uint8_t bb{0};
        int bits{0};

        if (a[0] == b[0]) {
            cpl += 8;
            a = moku::slice_expr<IP>(a, 1);
            b = moku::slice_expr<IP>(b, 1);
            continue;
        }
        bits = 8;
        std::tie(ab, bb) = std::tuple<uint8_t, uint8_t>(a[0], b[0]);
        while (true) {
            ab >>= 1;
            bb >>= 1;
            bits--;
            if (ab == bb) {
                cpl += bits;
                return;
            }
        }
    }
    return;
}
void init();
int main()
{
    init();
    confVal = &conf{{goos, runtime::GOOS}};
    lookupOrderName = std::map<hostLookupOrder, std::string>{
            {hostLookupCgo, "cgo"},
            {hostLookupFilesDNS, "files,dns"},
            {hostLookupDNSFiles, "dns,files"},
            {hostLookupFiles, "files"},
            {hostLookupDNS, "dns"}};
    defaultNS = moku::slice<std::string>{"127.0.0.1:53", "[::1]:53"};
    getHostname = os::Hostname;
    rr_mk