std::string Error::Error()
{
    return this->Op + " " + this->URL + ": " + this->Err.Error();
}
bool Error::Timeout()
{
    bool ok{false};
    timeout t{};

    std::tie(t, ok) = moku::try_assert<timeout>(this->Err);
    return ok && t.Timeout();
}
bool Error::Temporary()
{
    bool ok{false};
    temporary t{};

    std::tie(t, ok) = moku::try_assert<temporary>(this->Err);
    return ok && t.Temporary();
}
bool ishex(uint8_t c)
{
    if (('0' <= c && c <= '9')) {
    _ident_0_:
        return true;
    } else if (('a' <= c && c <= 'f')) {
    _ident_1_:
        return true;
    } else if (('A' <= c && c <= 'F')) {
    _ident_2_:
        return true;
    }
    return false;
}
uint8_t unhex(uint8_t c)
{
    if (('0' <= c && c <= '9')) {
    _ident_3_:
        return c - '0';
    } else if (('a' <= c && c <= 'f')) {
    _ident_4_:
        return c - 'a' + 10;
    } else if (('A' <= c && c <= 'F')) {
    _ident_5_:
        return c - 'A' + 10;
    }
    return 0;
}
std::string EscapeError::_ErrorByValue()
{
    return "invalid URL escape " + strconv::Quote(std::string(this));
}
std::string InvalidHostError::_ErrorByValue()
{
    return "invalid character " + strconv::Quote(std::string(this)) +
           " in host name";
}
bool shouldEscape(uint8_t c, encoding mode)
{
    if ('A' <= c && c <= 'Z' || 'a' <= c && c <= 'z' || '0' <= c && c <= '9') {
        return false;
    }
    if (mode == encodeHost || mode == encodeZone) {
        if ((c == '!') || (c == '$') || (c == '&') || (c == '\'') ||
            (c == '(') || (c == ')') || (c == '*') || (c == '+') ||
            (c == ',') || (c == ';') || (c == '=') || (c == ':') ||
            (c == '[') || (c == ']') || (c == '<') || (c == '>') ||
            (c == '"')) {
        _ident_6_:
            return false;
        }
    }
    if ((c == '-') || (c == '_') || (c == '.') || (c == '~')) {
    _ident_7_:
        return false;
    } else if ((c == '$') || (c == '&') || (c == '+') || (c == ',') ||
               (c == '/') || (c == ':') || (c == ';') || (c == '=') ||
               (c == '?') || (c == '@')) {
    _ident_8_:
        if ((mode == encodePath)) {
        _ident_9_:
            return c == '?';
        } else if ((mode == encodePathSegment)) {
        _ident_10_:
            return c == '/' || c == ';' || c == ',' || c == '?';
        } else if ((mode == encodeUserPassword)) {
        _ident_11_:
            return c == '@' || c == '/' || c == '?' || c == ':';
        } else if ((mode == encodeQueryComponent)) {
        _ident_12_:
            return true;
        } else if ((mode == encodeFragment)) {
        _ident_13_:
            return false;
        }
    }
    return true;
}
std::tuple<std::string, moku::error> QueryUnescape(std::string s)
{
    return unescape(s, encodeQueryComponent);
}
std::tuple<std::string, moku::error> PathUnescape(std::string s)
{
    return unescape(s, encodePathSegment);
}
std::tuple<std::string, moku::error> unescape(std::string s, encoding mode)
{
    bool hasPlus{false};
    int j{0};
    int n{0};
    moku::slice<uint8_t> t{};

    n = 0;
    hasPlus = false;
    {
        int i{0};
        for (i = 0; i < len(s);) {
            if ((s[i] == '%')) {
            _ident_14_:
                n++;
                if (i + 2 >= len(s) || !ishex(s[i + 1]) || !ishex(s[i + 2])) {
                    s = moku::slice_expr<std::string>(s, i);
                    if (len(s) > 3) {
                        s = moku::slice_expr<std::string>(s, 3);
                    }
                    return {"", EscapeError(s)};
                }
                if (mode == encodeHost && unhex(s[i + 1]) < 8 &&
                    moku::slice_expr<std::string>(s, i, i + 3) != "%25") {
                    return {"",
                            EscapeError(moku::slice_expr<std::string>(
                                    s, i, i + 3))};
                }
                if (mode == encodeZone) {
                    v = unhex(s[i + 1]) << 4 | unhex(s[i + 2]);
                    if (moku::slice_expr<std::string>(s, i, i + 3) != "%25" &&
                        v != ' ' && shouldEscape(v, encodeHost)) {
                        return {"",
                                EscapeError(moku::slice_expr<std::string>(
                                        s, i, i + 3))};
                    }
                }
                i += 3;
            } else if ((s[i] == '+')) {
            _ident_15_:
                hasPlus = mode == encodeQueryComponent;
                i++;
            } else {
            _ident_16_:
                if ((mode == encodeHost || mode == encodeZone) && s[i] < 0x80 &&
                    shouldEscape(s[i], mode)) {
                    return {"",
                            InvalidHostError(moku::slice_expr<std::string>(
                                    s, i, i + 1))};
                }
                i++;
            }
        }
    }
    if (n == 0 && !hasPlus) {
        return {s, nil};
    }
    t = make(moku::slice<byte>, len(s) - 2 * n);
    j = 0;
    {
        int i{0};
        for (i = 0; i < len(s);) {
            if ((s[i] == '%')) {
            _ident_17_:
                t[j] = unhex(s[i + 1]) << 4 | unhex(s[i + 2]);
                j++;
                i += 3;
            } else if ((s[i] == '+')) {
            _ident_18_:
                if (mode == encodeQueryComponent) {
                    t[j] = ' ';
                } else {
                    t[j] = '+';
                }
                j++;
                i++;
            } else {
            _ident_19_:
                t[j] = s[i];
                j++;
                i++;
            }
        }
    }
    return {std::string(t), nil};
}
std::string QueryEscape(std::string s)
{
    return escape(s, encodeQueryComponent);
}
std::string PathEscape(std::string s)
{
    return escape(s, encodePathSegment);
}
std::string escape(std::string s, encoding mode)
{
    int hexCount{0};
    int j{0};
    int spaceCount{0};
    moku::slice<uint8_t> t{};

    std::tie(spaceCount, hexCount) = std::tuple<int, int>(0, 0);
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            uint8_t c{0};

            c = s[i];
            if (shouldEscape(c, mode)) {
                if (c == ' ' && mode == encodeQueryComponent) {
                    spaceCount++;
                } else {
                    hexCount++;
                }
            }
        }
    }
    if (spaceCount == 0 && hexCount == 0) {
        return s;
    }
    t = make(moku::slice<byte>, len(s) + 2 * hexCount);
    j = 0;
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            {
                uint8_t c{0};
                c = s[i];
                if ((c == ' ' && mode == encodeQueryComponent)) {
                _ident_20_:
                    t[j] = '+';
                    j++;
                } else if ((shouldEscape(c, mode))) {
                _ident_21_:
                    t[j] = '%';
                    t[j + 1] = "0123456789ABCDEF"[c >> 4];
                    t[j + 2] = "0123456789ABCDEF"[c & 15];
                    j += 3;
                } else {
                _ident_22_:
                    t[j] = s[i];
                    j++;
                }
            }
        }
    }
    return std::string(t);
}
Userinfo *User(std::string username)
{
    return &Userinfo{username, "", false};
}
Userinfo *UserPassword(std::string username, std::string password)
{
    return &Userinfo{username, password, true};
}
std::string Userinfo::Username()
{
    return this->username;
}
std::tuple<std::string, bool> Userinfo::Password()
{
    return {this->password, this->passwordSet};
}
std::string Userinfo::String()
{
    std::string s{""};

    s = escape(this->username, encodeUserPassword);
    if (this->passwordSet) {
        s += ":" + escape(this->password, encodeUserPassword);
    }
    return s;
}
std::tuple<std::string, std::string, moku::error> getscheme(std::string rawurl)
{
    moku::error err{};
    std::string path{""};
    std::string scheme{""};

    {
        int i{0};
        for (i = 0; i < len(rawurl); i++) {
            uint8_t c{0};

            c = rawurl[i];
            if (('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z')) {
            _ident_23_:
            } else if (('0' <= c && c <= '9' || c == '+' || c == '-' ||
                        c == '.')) {
            _ident_24_:
                if (i == 0) {
                    return {"", rawurl, nil};
                }
            } else if ((c == ':')) {
            _ident_25_:
                if (i == 0) {
                    return {"", "", errors::New("missing protocol scheme")};
                }
                return {moku::slice_expr<std::string>(rawurl, i),
                        moku::slice_expr<std::string>(rawurl, i + 1),
                        nil};
            } else {
            _ident_26_:
                return {"", rawurl, nil};
            }
        }
    }
    return {"", rawurl, nil};
}
std::tuple<std::string, std::string> split(std::string s,
                                           std::string c,
                                           bool cutc)
{
    int i{0};

    i = strings::Index(s, c);
    if (i < 0) {
        return {s, ""};
    }
    if (cutc) {
        return {moku::slice_expr<std::string>(s, i),
                moku::slice_expr<std::string>(s, i + len(c))};
    }
    return {moku::slice_expr<std::string>(s, i),
            moku::slice_expr<std::string>(s, i)};
}
std::tuple<URL *, moku::error> Parse(std::string rawurl)
{
    moku::error err{};
    std::string frag{""};
    std::string u{""};
    URL *url{std::nullptr};

    std::tie(u, frag) = split(rawurl, "#", true);
    std::tie(url, err) = parse(u, false);
    if (!moku::is_nil(err)) {
        return {nil, &Error{"parse", u, err}};
    }
    if (frag == "") {
        return {url, nil};
    }
    {
        std::tie(url::Fragment, err) = unescape(frag, encodeFragment);

        if (!moku::is_nil(err)) {
            return {nil, &Error{"parse", rawurl, err}};
        }
    }
    return {url, nil};
}
std::tuple<URL *, moku::error> ParseRequestURI(std::string rawurl)
{
    moku::error err{};
    URL *url{std::nullptr};

    std::tie(url, err) = parse(rawurl, true);
    if (!moku::is_nil(err)) {
        return {nil, &Error{"parse", rawurl, err}};
    }
    return {url, nil};
}
std::tuple<URL *, moku::error> parse(std::string rawurl, bool viaRequest)
{
    moku::error err{};
    std::string rest{""};
    URL *url{std::nullptr};

    if (rawurl == "" && viaRequest) {
        return {nil, errors::New("empty url")};
    }
    url = new (URL);
    if (rawurl == "*") {
        url::Path = "*";
        return {url, nil};
    }
    {
        std::tie(url::Scheme, rest, err) = getscheme(rawurl);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    url::Scheme = strings::ToLower(url::Scheme);
    if (strings::HasSuffix(rest, "?") && strings::Count(rest, "?") == 1) {
        url::ForceQuery = true;
        rest = moku::slice_expr<std::string>(rest, len(rest) - 1);
    } else {
        std::tie(rest, url::RawQuery) = split(rest, "?", true);
    }
    if (!strings::HasPrefix(rest, "/")) {
        if (url::Scheme != "") {
            url::Opaque = rest;
            return {url, nil};
        }
        if (viaRequest) {
            return {nil, errors::New("invalid URI for request")};
        }
        colon = strings::Index(rest, ":");
        slash = strings::Index(rest, "/");
        if (colon >= 0 && (slash < 0 || colon < slash)) {
            return {nil,
                    errors::New(
                            "first path segment in URL cannot contain colon")};
        }
    }
    if ((url::Scheme != "" ||
         !viaRequest && !strings::HasPrefix(rest, "///")) &&
        strings::HasPrefix(rest, "//")) {
        std::tie(authority, rest) =
                split(moku::slice_expr<std::string>(rest, 2), "/", false);
        std::tie(url::User, url::Host, err) = parseAuthority(authority);
        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    {
        moku::error err{};

        err = url::setPath(rest);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    return {url, nil};
}
std::tuple<Userinfo *, std::string, moku::error> parseAuthority(
        std::string authority)
{
    moku::error err{};
    std::string host{""};
    int i{0};
    Userinfo *user{std::nullptr};
    std::string userinfo{""};

    i = strings::LastIndex(authority, "@");
    if (i < 0) {
        std::tie(host, err) = parseHost(authority);
    } else {
        std::tie(host, err) =
                parseHost(moku::slice_expr<std::string>(authority, i + 1));
    }
    if (!moku::is_nil(err)) {
        return {nil, "", err};
    }
    if (i < 0) {
        return {nil, host, nil};
    }
    userinfo = moku::slice_expr<std::string>(authority, i);
    if (!strings::Contains(userinfo, ":")) {
        {
            std::tie(userinfo, err) = unescape(userinfo, encodeUserPassword);

            if (!moku::is_nil(err)) {
                return {nil, "", err};
            }
        }
        user = User(userinfo);
    } else {
        std::tie(username, password) = split(userinfo, ":", true);
        {
            std::tie(username, err) = unescape(username, encodeUserPassword);

            if (!moku::is_nil(err)) {
                return {nil, "", err};
            }
        }
        {
            std::tie(password, err) = unescape(password, encodeUserPassword);

            if (!moku::is_nil(err)) {
                return {nil, "", err};
            }
        }
        user = UserPassword(username, password);
    }
    return {user, host, nil};
}
std::tuple<std::string, moku::error> parseHost(std::string host)
{
    moku::error err{};

    if (strings::HasPrefix(host, "[")) {
        i = strings::LastIndex(host, "]");
        if (i < 0) {
            return {"", errors::New("missing ']' in host")};
        }
        colonPort = moku::slice_expr<std::string>(host, i + 1);
        if (!validOptionalPort(colonPort)) {
            return {"", fmt::Errorf("invalid port %q after host", colonPort)};
        }
        zone = strings::Index(moku::slice_expr<std::string>(host, i), "%25");
        if (zone >= 0) {
            std::tie(host1, err) = unescape(
                    moku::slice_expr<std::string>(host, zone), encodeHost);
            if (!moku::is_nil(err)) {
                return {"", err};
            }
            std::tie(host2, err) = unescape(
                    moku::slice_expr<std::string>(host, zone, i), encodeZone);
            if (!moku::is_nil(err)) {
                return {"", err};
            }
            std::tie(host3, err) = unescape(
                    moku::slice_expr<std::string>(host, i), encodeHost);
            if (!moku::is_nil(err)) {
                return {"", err};
            }
            return {host1 + host2 + host3, nil};
        }
    }
    {
        std::tie(host, err) = unescape(host, encodeHost);

        if (!moku::is_nil(err)) {
            return {"", err};
        }
    }
    return {host, nil};
}
moku::error URL::setPath(std::string p)
{
    moku::error err{};
    std::string path{""};

    std::tie(path, err) = unescape(p, encodePath);
    if (!moku::is_nil(err)) {
        return err;
    }
    this->Path = path;
    {
        std::string escp{""};

        escp = escape(path, encodePath);

        if (p == escp) {
            this->RawPath = "";
        } else {
            this->RawPath = p;
        }
    }
    return nil;
}
std::string URL::EscapedPath()
{
    if (this->RawPath != "" && validEncodedPath(this->RawPath)) {
        std::tie(p, err) = unescape(this->RawPath, encodePath);
        if (moku::is_nil(err) && p == this->Path) {
            return this->RawPath;
        }
    }
    if (this->Path == "*") {
        return "*";
    }
    return escape(this->Path, encodePath);
}
bool validEncodedPath(std::string s)
{
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            if ((s[i] == '!') || (s[i] == '$') || (s[i] == '&') ||
                (s[i] == '\'') || (s[i] == '(') || (s[i] == ')') ||
                (s[i] == '*') || (s[i] == '+') || (s[i] == ',') ||
                (s[i] == ';') || (s[i] == '=') || (s[i] == ':') ||
                (s[i] == '@')) {
            _ident_27_:
            } else if ((s[i] == '[') || (s[i] == ']')) {
            _ident_28_:
            } else if ((s[i] == '%')) {
            _ident_29_:
            } else {
            _ident_30_:
                if (shouldEscape(s[i], encodePath)) {
                    return false;
                }
            }
        }
    }
    return true;
}
bool validOptionalPort(std::string port)
{
    if (port == "") {
        return true;
    }
    if (port[0] != ':') {
        return false;
    }
    {
        int32_t b{0};
        for (b : moku::range_value<std::string>(
                     moku::slice_expr<std::string>(port, 1))) {
            if (b < '0' || b > '9') {
                return false;
            }
        }
    }
    return true;
}
std::string URL::String()
{
    Buffer buf{};

    if (this->Scheme != "") {
        buf.WriteString(this->Scheme);
        buf.WriteByte(':');
    }
    if (this->Opaque != "") {
        buf.WriteString(this->Opaque);
    } else {
        if (this->Scheme != "" || this->Host != "" ||
            !moku::is_nil(this->User)) {
            buf.WriteString("//");
            {
                Userinfo *ui{std::nullptr};

                ui = this->User;

                if (!moku::is_nil(ui)) {
                    buf.WriteString(ui->String());
                    buf.WriteByte('@');
                }
            }
            {
                std::string h{""};

                h = this->Host;

                if (h != "") {
                    buf.WriteString(escape(h, encodeHost));
                }
            }
        }
        path = this->EscapedPath();
        if (path != "" && path[0] != '/' && this->Host != "") {
            buf.WriteByte('/');
        }
        if (buf.Len() == 0) {
            {
                int i{0};

                i = strings::IndexByte(path, ':');

                if (i > -1 &&
                    strings::IndexByte(moku::slice_expr<std::string>(path, i),
                                       '/') == -1) {
                    buf.WriteString("./");
                }
            }
        }
        buf.WriteString(path);
    }
    if (this->ForceQuery || this->RawQuery != "") {
        buf.WriteByte('?');
        buf.WriteString(this->RawQuery);
    }
    if (this->Fragment != "") {
        buf.WriteByte('#');
        buf.WriteString(escape(this->Fragment, encodeFragment));
    }
    return buf.String();
}
std::string Values::_GetByValue(std::string key)
{
    moku::slice<std::string> vs{};

    if (moku::is_nil(this)) {
        return "";
    }
    vs = this[key];
    if (len(vs) == 0) {
        return "";
    }
    return vs[0];
}
void Values::_SetByValue(std::string key, std::string value)
{
    this[key] = moku::slice<std::string>{value};
}
void Values::_AddByValue(std::string key, std::string value)
{
    this[key] = append(this[key], value);
}
void Values::_DelByValue(std::string key)
{
    delete (this, key);
}
std::tuple<Values, moku::error> ParseQuery(std::string query)
{
    moku::error err{};
    Values m{};

    m = make(Values);
    err = parseQuery(m, query);
    return {m, err};
}
moku::error parseQuery(Values m, std::string query)
{
    moku::error err{};

    while (query != "") {
        moku::error err1{};
        std::string key{""};
        std::string value{""};

        key = query;
        {
            int i{0};

            i = strings::IndexAny(key, "&;");

            if (i >= 0) {
                std::tie(key, query) = std::tuple<std::string, std::string>(
                        moku::slice_expr<std::string>(key, i),
                        moku::slice_expr<std::string>(key, i + 1));
            } else {
                query = "";
            }
        }
        if (key == "") {
            continue;
        }
        value = "";
        {
            int i{0};

            i = strings::Index(key, "=");

            if (i >= 0) {
                std::tie(key, value) = std::tuple<std::string, std::string>(
                        moku::slice_expr<std::string>(key, i),
                        moku::slice_expr<std::string>(key, i + 1));
            }
        }
        std::tie(key, err1) = QueryUnescape(key);
        if (!moku::is_nil(err1)) {
            if (moku::is_nil(err)) {
                err = err1;
            }
            continue;
        }
        std::tie(value, err1) = QueryUnescape(value);
        if (!moku::is_nil(err1)) {
            if (moku::is_nil(err)) {
                err = err1;
            }
            continue;
        }
        m[key] = append(m[key], value);
    }
    return err;
}
std::string Values::_EncodeByValue()
{
    Buffer buf{};
    moku::slice<std::string> keys{};

    if (moku::is_nil(this)) {
        return "";
    }
    keys = make(moku::slice<std::string>, 0, len(this));
    {
        std::string k{""};
        for (k : moku::range_key<Values>(this)) {
            keys = append(keys, k);
        }
    }
    sort::Strings(keys);
    {
        std::string k{""};
        for (k : moku::range_value<moku::slice<std::string>>(keys)) {
            vs = this[k];
            prefix = QueryEscape(k) + "=";
            {
                std::string v{""};
                for (v : moku::range_value<moku::slice<std::string>>(vs)) {
                    if (buf.Len() > 0) {
                        buf.WriteByte('&');
                    }
                    buf.WriteString(prefix);
                    buf.WriteString(QueryEscape(this));
                }
            }
        }
    }
    return buf.String();
}
std::string resolvePath(std::string base, std::string ref)
{
    moku::slice<std::string> dst{};
    std::string full{""};
    moku::slice<std::string> src{};

    if (ref == "") {
        full = base;
    } else {
        if (ref[0] != '/') {
            i = strings::LastIndex(base, "/");
            full = moku::slice_expr<std::string>(base, i + 1) + ref;
        } else {
            full = ref;
        }
    }
    if (full == "") {
        return "";
    }
    src = strings::Split(full, "/");
    {
        std::string elem{""};
        for (elem : moku::range_value<moku::slice<std::string>>(src)) {
            if ((elem == ".")) {
            _ident_31_:
            } else if ((elem == "..")) {
            _ident_32_:
                if (len(dst) > 0) {
                    dst = moku::slice_expr<moku::slice<std::string>>(
                            dst, len(dst) - 1);
                }
            } else {
            _ident_33_:
                dst = append(dst, elem);
            }
        }
    }
    {
        std::string last{""};

        last = src[len(src) - 1];

        if (last == "." || last == "..") {
            dst = append(dst, "");
        }
    }
    return "/" + strings::TrimLeft(strings::Join(dst, "/"), "/");
}
bool URL::IsAbs()
{
    return this->Scheme != "";
}
std::tuple<URL *, moku::error> URL::Parse(std::string ref)
{
    moku::error err{};
    URL *refurl{std::nullptr};

    std::tie(refurl, err) = Parse(ref);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    return {this->ResolveReference(refurl), nil};
}
URL *URL::ResolveReference(URL *ref)
{
    URL url{};

    url = *ref;
    if (ref->Scheme == "") {
        url::Scheme = this->Scheme;
    }
    if (ref->Scheme != "" || ref->Host != "" || !moku::is_nil(ref->User)) {
        url::setPath(resolvePath(ref->EscapedPath(), ""));
        return &url;
    }
    if (ref->Opaque != "") {
        url::User = nil;
        url::Host = "";
        url::Path = "";
        return &url;
    }
    if (ref->Path == "") {
        if (ref->RawQuery == "") {
            url::RawQuery = this->RawQuery;
            if (ref->Fragment == "") {
                url::Fragment = this->Fragment;
            }
        }
    }
    url::Host = this->Host;
    url::User = this->User;
    url::setPath(resolvePath(this->EscapedPath(), ref->EscapedPath()));
    return &url;
}
Values URL::Query()
{
    Values v{};

    std::tie(v, _) = ParseQuery(this->RawQuery);
    return v;
}
std::string URL::RequestURI()
{
    std::string result{""};

    result = this->Opaque;
    if (result == "") {
        result = this->EscapedPath();
        if (result == "") {
            result = "/";
        }
    } else {
        if (strings::HasPrefix(result, "//")) {
            result = this->Scheme + ":" + result;
        }
    }
    if (this->ForceQuery || this->RawQuery != "") {
        result += "?" + this->RawQuery;
    }
    return result;
}
std::string URL::Hostname()
{
    return stripPort(this->Host);
}
std::string URL::Port()
{
    return portOnly(this->Host);
}
std::string stripPort(std::string hostport)
{
    int colon{0};

    colon = strings::IndexByte(hostport, ':');
    if (colon == -1) {
        return hostport;
    }
    {
        int i{0};

        i = strings::IndexByte(hostport, ']');

        if (i != -1) {
            return strings::TrimPrefix(
                    moku::slice_expr<std::string>(hostport, i), "[");
        }
    }
    return moku::slice_expr<std::string>(hostport, colon);
}
std::string portOnly(std::string hostport)
{
    int colon{0};

    colon = strings::IndexByte(hostport, ':');
    if (colon == -1) {
        return "";
    }
    {
        int i{0};

        i = strings::Index(hostport, "]:");

        if (i != -1) {
            return moku::slice_expr<std::string>(hostport, i + len("]:"));
        }
    }
    if (strings::Contains(hostport, "]")) {
        return "";
    }
    return moku::slice_expr<std::string>(hostport, colon + len(":"));
}
std::tuple<moku::slice<uint8_t>, moku::error> URL::MarshalBinary()
{
    moku::error err{};
    moku::slice<uint8_t> text{};

    return {moku::slice<byte>(this->String()), nil};
}
moku::error URL::UnmarshalBinary(moku::slice<uint8_t> text)
{
    moku::error err{};
    URL *u1{std::nullptr};

    std::tie(u1, err) = Parse(std::string(text));
    if (!moku::is_nil(err)) {
        return err;
    }
    *this = *u1;
    return nil;
}
int main()
{
    _main();
    return 0;
}
