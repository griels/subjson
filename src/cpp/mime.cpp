std::string WordEncoder::_EncodeByValue(std::string charset, std::string s)
{
    if (!needsEncoding(s)) {
        return s;
    }
    return this->encodeWord(charset, s);
}
bool needsEncoding(std::string s)
{
    {
        int32_t b{0};
        for (b : moku::range_value<std::string>(s)) {
            if ((b < ' ' || b > '~') && b != '\t') {
                return true;
            }
        }
    }
    return false;
}
std::string WordEncoder::_encodeWordByValue(std::string charset, std::string s)
{
    moku::defer _defer_;
    Buffer *buf{std::nullptr};

    buf = getBuffer();
    _defer_.Push([=]() mutable { putBuffer(buf); });
    this->openWord(buf, charset);
    if (this == BEncoding) {
        this->bEncode(buf, charset, s);
    } else {
        this->qEncode(buf, charset, s);
    }
    closeWord(buf);
    return buf->String();
}
void WordEncoder::_bEncodeByValue(Buffer *buf,
                                  std::string charset,
                                  std::string s)
{
    int currentLen{0};
    int last{0};
    int runeLen{0};
    WriteCloser w{};

    w = base64::NewEncoder(base64::StdEncoding, buf);
    if (!isUTF8(charset) ||
        base64::StdEncoding->EncodedLen(len(s)) <= maxContentLen) {
        io::WriteString(w, s);
        w.Close();
        return;
    }
    {
        int i{0};
        for (i = 0; i < len(s); i += runeLen) {
            std::tie(_, runeLen) = utf8::DecodeRuneInString(
                    moku::slice_expr<std::string>(s, i));
            if (currentLen + runeLen <= maxBase64Len) {
                currentLen += runeLen;
            } else {
                io::WriteString(w, moku::slice_expr<std::string>(s, last, i));
                w.Close();
                this->splitWord(buf, charset);
                last = i;
                currentLen = runeLen;
            }
        }
    }
    io::WriteString(w, moku::slice_expr<std::string>(s, last));
    w.Close();
}
void WordEncoder::_qEncodeByValue(Buffer *buf,
                                  std::string charset,
                                  std::string s)
{
    int currentLen{0};
    int runeLen{0};

    if (!isUTF8(charset)) {
        writeQString(buf, s);
        return;
    }
    {
        int i{0};
        for (i = 0; i < len(s); i += runeLen) {
            uint8_t b{0};
            int encLen{0};

            b = s[i];
            if (b >= ' ' && b <= '~' && b != '=' && b != '?' && b != '_') {
                std::tie(runeLen, encLen) = std::tuple<int, int>(1, 1);
            } else {
                std::tie(_, runeLen) = utf8::DecodeRuneInString(
                        moku::slice_expr<std::string>(s, i));
                encLen = 3 * runeLen;
            }
            if (currentLen + encLen > maxContentLen) {
                this->splitWord(buf, charset);
                currentLen = 0;
            }
            writeQString(buf, moku::slice_expr<std::string>(s, i, i + runeLen));
            currentLen += encLen;
        }
    }
}
void writeQString(Buffer *buf, std::string s)
{
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            {
                uint8_t b{0};
                b = s[i];
                if ((b == ' ')) {
                _ident_0_:
                    buf->WriteByte('_');
                } else if ((b >= '!' && b <= '~' && b != '=' && b != '?' &&
                            b != '_')) {
                _ident_1_:
                    buf->WriteByte(b);
                } else {
                _ident_2_:
                    buf->WriteByte('=');
                    buf->WriteByte(upperhex[b >> 4]);
                    buf->WriteByte(upperhex[b & 0x0f]);
                }
            }
        }
    }
}
void WordEncoder::_openWordByValue(Buffer *buf, std::string charset)
{
    buf->WriteString("=?");
    buf->WriteString(charset);
    buf->WriteByte('?');
    buf->WriteByte(byte(this));
    buf->WriteByte('?');
}
void closeWord(Buffer *buf)
{
    buf->WriteString("?=");
}
void WordEncoder::_splitWordByValue(Buffer *buf, std::string charset)
{
    closeWord(buf);
    buf->WriteByte(' ');
    this->openWord(buf, charset);
}
bool isUTF8(std::string charset)
{
    return strings::EqualFold(charset, "UTF-8");
}
std::tuple<std::string, moku::error> WordDecoder::Decode(std::string word)
{
    moku::defer _defer_;
    Buffer *buf{std::nullptr};
    std::string charset{""};
    moku::slice<uint8_t> content{};
    uint8_t encoding{0};
    moku::error err{};
    int split{0};
    std::string text{""};

    if (len(word) < 8 || !strings::HasPrefix(word, "=?") ||
        !strings::HasSuffix(word, "?=") || strings::Count(word, "?") != 4) {
        return {"", errInvalidWord};
    }
    word = moku::slice_expr<std::string>(word, 2, len(word) - 2);
    split = strings::IndexByte(word, '?');
    charset = moku::slice_expr<std::string>(word, split);
    if (len(charset) == 0) {
        return {"", errInvalidWord};
    }
    if (len(word) < split + 3) {
        return {"", errInvalidWord};
    }
    encoding = word[split + 1];
    if (word[split + 2] != '?') {
        return {"", errInvalidWord};
    }
    text = moku::slice_expr<std::string>(word, split + 3);
    std::tie(content, err) = decode(encoding, text);
    if (!moku::is_nil(err)) {
        return {"", err};
    }
    buf = getBuffer();
    _defer_.Push([=]() mutable { putBuffer(buf); });
    {
        moku::error err{};

        err = this->convert(buf, charset, content);

        if (!moku::is_nil(err)) {
            return {"", err};
        }
    }
    return {buf->String(), nil};
}
std::tuple<std::string, moku::error> WordDecoder::DecodeHeader(
        std::string header)
{
    moku::defer _defer_;
    bool betweenWords{false};
    Buffer *buf{std::nullptr};
    int i{0};

    i = strings::Index(header, "=?");
    if (i == -1) {
        return {header, nil};
    }
    buf = getBuffer();
    _defer_.Push([=]() mutable { putBuffer(buf); });
    buf->WriteString(moku::slice_expr<std::string>(header, i));
    header = moku::slice_expr<std::string>(header, i);
    betweenWords = false;
    while (true) {
        std::string charset{""};
        moku::slice<uint8_t> content{};
        int cur{0};
        uint8_t encoding{0};
        int end{0};
        moku::error err{};
        int i{0};
        int j{0};
        int start{0};
        std::string text{""};

        start = strings::Index(header, "=?");
        if (start == -1) {
            break;
        }
        cur = start + len("=?");
        i = strings::Index(moku::slice_expr<std::string>(header, cur), "?");
        if (i == -1) {
            break;
        }
        charset = moku::slice_expr<std::string>(header, cur, cur + i);
        cur += i + len("?");
        if (len(header) < cur + len("Q??=")) {
            break;
        }
        encoding = header[cur];
        cur++;
        if (header[cur] != '?') {
            break;
        }
        cur++;
        j = strings::Index(moku::slice_expr<std::string>(header, cur), "?=");
        if (j == -1) {
            break;
        }
        text = moku::slice_expr<std::string>(header, cur, cur + j);
        end = cur + j + len("?=");
        std::tie(content, err) = decode(encoding, text);
        if (!moku::is_nil(err)) {
            betweenWords = false;
            buf->WriteString(moku::slice_expr<std::string>(header, start + 2));
            header = moku::slice_expr<std::string>(header, start + 2);
            continue;
        }
        if (start > 0 &&
            (!betweenWords ||
             hasNonWhitespace(moku::slice_expr<std::string>(header, start)))) {
            buf->WriteString(moku::slice_expr<std::string>(header, start));
        }
        {
            moku::error err{};

            err = this->convert(buf, charset, content);

            if (!moku::is_nil(err)) {
                return {"", err};
            }
        }
        header = moku::slice_expr<std::string>(header, end);
        betweenWords = true;
    }
    if (len(header) > 0) {
        buf->WriteString(header);
    }
    return {buf->String(), nil};
}
std::tuple<moku::slice<uint8_t>, moku::error> decode(uint8_t encoding,
                                                     std::string text)
{
    if ((encoding == 'B') || (encoding == 'b')) {
    _ident_3_:
        return base64::StdEncoding->DecodeString(text);
    } else if ((encoding == 'Q') || (encoding == 'q')) {
    _ident_4_:
        return qDecode(text);
    } else {
    _ident_5_:
        return {nil, errInvalidWord};
    }
}
moku::error WordDecoder::convert(Buffer *buf,
                                 std::string charset,
                                 moku::slice<uint8_t> content)
{
    if ((strings::EqualFold("utf-8", charset))) {
    _ident_6_:
        buf->Write(content);
    } else if ((strings::EqualFold("iso-8859-1", charset))) {
    _ident_7_ : {
        uint8_t c{0};
        for (c : moku::range_value<moku::slice<uint8_t>>(content)) {
            buf->WriteRune(rune(c));
        }
    }
    } else if ((strings::EqualFold("us-ascii", charset))) {
    _ident_8_ : {
        uint8_t c{0};
        for (c : moku::range_value<moku::slice<uint8_t>>(content)) {
            if (c >= utf8::RuneSelf) {
                buf->WriteRune(unicode::ReplacementChar);
            } else {
                buf->WriteByte(c);
            }
        }
    }
    } else {
    _ident_9_:
        if (moku::is_nil(this->CharsetReader)) {
            return fmt::Errorf("mime: unhandled charset %q", charset);
        }
        std::tie(r, err) = this->CharsetReader(strings::ToLower(charset),
                                               bytes::NewReader(content));
        if (!moku::is_nil(err)) {
            return err;
        }
        {
            std::tie(_, err) = buf->ReadFrom(r);

            if (!moku::is_nil(err)) {
                return err;
            }
        }
    }
    return nil;
}
bool hasNonWhitespace(std::string s)
{
    {
        int32_t b{0};
        for (b : moku::range_value<std::string>(s)) {
            if ((b == ' ') || (b == '\t') || (b == '\n') || (b == '\r')) {
            _ident_10_:
            } else {
            _ident_11_:
                return true;
            }
        }
    }
    return false;
}
std::tuple<moku::slice<uint8_t>, moku::error> qDecode(std::string s)
{
    moku::slice<uint8_t> dec{};
    int n{0};

    dec = make(moku::slice<byte>, len(s));
    n = 0;
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            {
                uint8_t c{0};
                c = s[i];
                if ((c == '_')) {
                _ident_12_:
                    dec[n] = ' ';
                } else if ((c == '=')) {
                _ident_13_:
                    if (i + 2 >= len(s)) {
                        return {nil, errInvalidWord};
                    }
                    std::tie(b, err) = readHexByte(s[i + 1], s[i + 2]);
                    if (!moku::is_nil(err)) {
                        return {nil, err};
                    }
                    dec[n] = b;
                    i += 2;
                } else if (((c <= '~' && c >= ' ') || c == '\n' || c == '\r' ||
                            c == '\t')) {
                _ident_14_:
                    dec[n] = c;
                } else {
                _ident_15_:
                    return {nil, errInvalidWord};
                }
            }
            n++;
        }
    }
    return {moku::slice_expr<moku::slice<uint8_t>>(dec, n), nil};
}
std::tuple<uint8_t, moku::error> readHexByte(uint8_t a, uint8_t b)
{
    moku::error err{};
    uint8_t hb{0};
    uint8_t lb{0};

    {
        std::tie(hb, err) = fromHex(a);

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    {
        std::tie(lb, err) = fromHex(b);

        if (!moku::is_nil(err)) {
            return {0, err};
        }
    }
    return {hb << 4 | lb, nil};
}
std::tuple<uint8_t, moku::error> fromHex(uint8_t b)
{
    if ((b >= '0' && b <= '9')) {
    _ident_16_:
        return {b - '0', nil};
    } else if ((b >= 'A' && b <= 'F')) {
    _ident_17_:
        return {b - 'A' + 10, nil};
    } else if ((b >= 'a' && b <= 'f')) {
    _ident_18_:
        return {b - 'a' + 10, nil};
    }
    return {0, fmt::Errorf("mime: invalid hex byte %#02x", b)};
}
Buffer *getBuffer()
{
    return moku::try_assert<*bytes::Buffer>(bufPool.Get());
}
void putBuffer(Buffer *buf)
{
    if (buf->Len() > 1024) {
        return;
    }
    buf->Reset();
    bufPool.Put(moku::make_iface<Buffer *>(buf));
}
void init();
int main()
{
    init();
    errInvalidWord = errors::New("mime: invalid RFC 2047 encoded-word");
    maxBase64Len = base64::StdEncoding->DecodedLen(maxContentLen);
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidMediaParameter = errors::New("mime: invalid media parameter");
    builtinTypesLower = std::map<std::string, std::string>{
            {".css", "text/css; charset=utf-8"},
            {".gif", "image/gif"},
            {".htm", "text/html; charset=utf-8"},
            {".html", "text/html; charset=utf-8"},
            {".jpg", "image/jpeg"},
            {".js", "application/x-javascript"},
            {".pdf", "application/pdf"},
            {".png", "image/png"},
            {".svg", "image/svg+xml"},
            {".xml", "text/xml; charset=utf-8"}};
    typeFiles = moku::slice<std::string>{"/etc/mime.types",
                                         "/etc/apache2/mime.types",
                                         "/etc/apache/mime.types"};
    _main();
    return 0;
}
bool isTSpecial(int32_t r)
{
return strings::ContainsRune(`()<>@,;:\"/[]?=`, r);
}
bool isTokenChar(int32_t r)
{
    return r > 0x20 && r < 0x7f && !isTSpecial(r);
}
bool isToken(std::string s)
{
    if (s == "") {
        return false;
    }
    return strings::IndexFunc(s, isNotTokenChar) < 0;
}
void init();
int main()
{
    init();
    errInvalidWord = errors::New("mime: invalid RFC 2047 encoded-word");
    maxBase64Len = base64::StdEncoding->DecodedLen(maxContentLen);
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidMediaParameter = errors::New("mime: invalid media parameter");
    builtinTypesLower = std::map<std::string, std::string>{
            {".css", "text/css; charset=utf-8"},
            {".gif", "image/gif"},
            {".htm", "text/html; charset=utf-8"},
            {".html", "text/html; charset=utf-8"},
            {".jpg", "image/jpeg"},
            {".js", "application/x-javascript"},
            {".pdf", "application/pdf"},
            {".png", "image/png"},
            {".svg", "image/svg+xml"},
            {".xml", "text/xml; charset=utf-8"}};
    typeFiles = moku::slice<std::string>{"/etc/mime.types",
                                         "/etc/apache2/mime.types",
                                         "/etc/apache/mime.types"};
    _main();
    return 0;
}
std::string FormatMediaType(std::string t,
                            std::map<std::string, std::string> param)
{
    moku::slice<std::string> attrs{};
    Buffer b{};

    {
        int slash{0};

        slash = strings::Index(t, "/");

        if (slash == -1) {
            if (!isToken(t)) {
                return "";
            }
            b.WriteString(strings::ToLower(t));
        } else {
            std::tie(major, sub) = std::tuple<std::string, std::string>(
                    moku::slice_expr<std::string>(t, slash),
                    moku::slice_expr<std::string>(t, slash + 1));
            if (!isToken(major) || !isToken(sub)) {
                return "";
            }
            b.WriteString(strings::ToLower(major));
            b.WriteByte('/');
            b.WriteString(strings::ToLower(sub));
        }
    }
    attrs = make(moku::slice<std::string>, 0, len(param));
    {
        std::string a{""};
        for (a : moku::range_key<std::map<std::string, std::string>>(param)) {
            attrs = append(attrs, a);
        }
    }
    sort::Strings(attrs);
    {
        std::string attribute{""};
        for (attribute : moku::range_value<moku::slice<std::string>>(attrs)) {
            value = param[attribute];
            b.WriteByte(';');
            b.WriteByte(' ');
            if (!isToken(attribute)) {
                return "";
            }
            b.WriteString(strings::ToLower(attribute));
            b.WriteByte('=');
            if (isToken(value)) {
                b.WriteString(value);
                continue;
            }
            b.WriteByte('"');
            offset = 0;
            {
                int32_t character{0};
                int index{0};
                for (std::tie(index, character) :
                     moku::range_key_value<std::string>(value)) {
                    if (character == '"' || character == '\\') {
                        b.WriteString(moku::slice_expr<std::string>(
                                value, offset, index));
                        offset = index;
                        b.WriteByte('\\');
                    }
                    if (character & 0x80 != 0) {
                        return "";
                    }
                }
            }
            b.WriteString(moku::slice_expr<std::string>(value, offset));
            b.WriteByte('"');
        }
    }
    return b.String();
}
moku::error checkMediaTypeDisposition(std::string s)
{
    std::string rest{""};
    std::string subtype{""};
    std::string typ{""};

    std::tie(typ, rest) = consumeToken(s);
    if (typ == "") {
        return errors::New("mime: no media type");
    }
    if (rest == "") {
        return nil;
    }
    if (!strings::HasPrefix(rest, "/")) {
        return errors::New("mime: expected slash after first token");
    }
    std::tie(subtype, rest) =
            consumeToken(moku::slice_expr<std::string>(rest, 1));
    if (subtype == "") {
        return errors::New("mime: expected token after slash");
    }
    if (rest != "") {
        return errors::New("mime: unexpected content after media subtype");
    }
    return nil;
}
std::tuple<std::string, std::map<std::string, std::string>, moku::error>
ParseMediaType(std::string v)
{
    Buffer buf{};
    std::map<std::string, std::map<std::string, std::string>> continuation{};
    moku::error err{};
    int i{0};
    std::string mediatype{""};
    std::map<std::string, std::string> params{};

    i = strings::Index(v, ";");
    if (i == -1) {
        i = len(v);
    }
    mediatype = strings::TrimSpace(
            strings::ToLower(moku::slice_expr<std::string>(v, 0, i)));
    err = checkMediaTypeDisposition(mediatype);
    if (!moku::is_nil(err)) {
        return {"", nil, err};
    }
    params = make(std::map<std::string, std::string>);
    v = moku::slice_expr<std::string>(v, i);
    while (len(v) > 0) {
        std::string key{""};
        std::map<std::string, std::string> pmap{};
        std::string rest{""};
        std::string value{""};

        v = strings::TrimLeftFunc(v, unicode::IsSpace);
        if (len(v) == 0) {
            break;
        }
        std::tie(key, value, rest) = consumeMediaParam(v);
        if (key == "") {
            if (strings::TrimSpace(rest) == ";") {
                return;
            }
            return {mediatype, nil, ErrInvalidMediaParameter};
        }
        pmap = params;
        {
            int idx{0};

            idx = strings::Index(key, "*");

            if (idx != -1) {
                baseName = moku::slice_expr<std::string>(key, idx);
                if (moku::is_nil(continuation)) {
                    continuation =
                            make(std::map<std::string,
                                          std::map<std::string, std::string>>);
                }
                {
                    std::tie(pmap, ok) = continuation[baseName];

                    if (!ok) {
                        continuation[baseName] =
                                make(std::map<std::string, std::string>);
                        pmap = continuation[baseName];
                    }
                }
            }
        }
        {
            bool exists{false};

            std::tie(_, exists) = pmap[key];

            if (exists) {
                return {"", nil, errors::New("mime: duplicate parameter name")};
            }
        }
        pmap[key] = value;
        v = rest;
    }
    {
        std::string key{""};
        std::map<std::string, std::string> pieceMap{};
        for (std::tie(key, pieceMap) : moku::range_key_value<
                     std::map<std::string, std::map<std::string, std::string>>>(
                     continuation)) {
            singlePartKey = key + "*";
            {
                bool ok{false};
                std::string v{""};

                std::tie(v, ok) = pieceMap[singlePartKey];

                if (ok) {
                    decv = decode2231Enc(v);
                    params[key] = decv;
                    continue;
                }
            }
            buf.Reset();
            valid = false;
            {
                int n{0};
                for (n = 0;; n++) {
                    std::string encodedPart{""};
                    std::string simplePart{""};

                    simplePart = fmt::Sprintf("%s*%d", key, n);
                    {
                        bool ok{false};
                        std::string v{""};

                        std::tie(v, ok) = pieceMap[simplePart];

                        if (ok) {
                            valid = true;
                            buf.WriteString(v);
                            continue;
                        }
                    }
                    encodedPart = simplePart + "*";
                    {
                        bool ok{false};
                        std::string v{""};

                        std::tie(v, ok) = pieceMap[encodedPart];

                        if (ok) {
                            valid = true;
                            if (n == 0) {
                                buf.WriteString(decode2231Enc(v));
                            } else {
                                std::tie(decv, _) = percentHexUnescape(v);
                                buf.WriteString(decv);
                            }
                        } else {
                            break;
                        }
                    }
                }
            }
            if (valid) {
                params[key] = buf.String();
            }
        }
    }
    return;
}
std::string decode2231Enc(std::string v)
{
    std::string charset{""};
    std::string encv{""};
    moku::slice<std::string> sv{};

    sv = strings::SplitN(v, "'", 3);
    if (len(sv) != 3) {
        return "";
    }
    charset = strings::ToLower(sv[0]);
    if (charset != "us-ascii" && charset != "utf-8") {
        return "";
    }
    std::tie(encv, _) = percentHexUnescape(sv[2]);
    return encv;
}
bool isNotTokenChar(int32_t r)
{
    return !isTokenChar(r);
}
std::tuple<std::string, std::string> consumeToken(std::string v)
{
    int notPos{0};
    std::string rest{""};
    std::string token{""};

    notPos = strings::IndexFunc(v, isNotTokenChar);
    if (notPos == -1) {
        return {v, ""};
    }
    if (notPos == 0) {
        return {"", v};
    }
    return {moku::slice_expr<std::string>(v, 0, notPos),
            moku::slice_expr<std::string>(v, notPos)};
}
std::tuple<std::string, std::string> consumeValue(std::string v)
{
    Buffer *buffer{std::nullptr};
    std::string rest{""};
    std::string value{""};

    if (v == "") {
        return;
    }
    if (v[0] != '"') {
        return consumeToken(v);
    }
    buffer = new (bytes::Buffer);
    {
        int i{0};
        for (i = 1; i < len(v); i++) {
            uint8_t r{0};

            r = v[i];
            if (r == '"') {
                return {buffer->String(),
                        moku::slice_expr<std::string>(v, i + 1)};
            }
            if (r == '\\' && i + 1 < len(v) && !isTokenChar(rune(v[i + 1]))) {
                buffer->WriteByte(v[i + 1]);
                i++;
                continue;
            }
            if (r == '\r' || r == '\n') {
                return {"", v};
            }
            buffer->WriteByte(v[i]);
        }
    }
    return {"", v};
}
std::tuple<std::string, std::string, std::string> consumeMediaParam(
        std::string v)
{
    std::string param{""};
    std::string rest{""};
    std::string rest2{""};
    std::string value{""};

    rest = strings::TrimLeftFunc(v, unicode::IsSpace);
    if (!strings::HasPrefix(rest, ";")) {
        return {"", "", v};
    }
    rest = moku::slice_expr<std::string>(rest, 1);
    rest = strings::TrimLeftFunc(rest, unicode::IsSpace);
    std::tie(param, rest) = consumeToken(rest);
    param = strings::ToLower(param);
    if (param == "") {
        return {"", "", v};
    }
    rest = strings::TrimLeftFunc(rest, unicode::IsSpace);
    if (!strings::HasPrefix(rest, "=")) {
        return {"", "", v};
    }
    rest = moku::slice_expr<std::string>(rest, 1);
    rest = strings::TrimLeftFunc(rest, unicode::IsSpace);
    std::tie(value, rest2) = consumeValue(rest);
    if (value == "" && rest2 == rest) {
        return {"", "", v};
    }
    rest = rest2;
    return {param, value, rest};
}
std::tuple<std::string, moku::error> percentHexUnescape(std::string s)
{
    int j{0};
    int percents{0};
    moku::slice<uint8_t> t{};

    percents = 0;
    {
        int i{0};
        for (i = 0; i < len(s);) {
            if (s[i] != '%') {
                i++;
                continue;
            }
            percents++;
            if (i + 2 >= len(s) || !ishex(s[i + 1]) || !ishex(s[i + 2])) {
                s = moku::slice_expr<std::string>(s, i);
                if (len(s) > 3) {
                    s = moku::slice_expr<std::string>(s, 0, 3);
                }
                return {"",
                        fmt::Errorf("mime: bogus characters after %%: %q", s)};
            }
            i += 3;
        }
    }
    if (percents == 0) {
        return {s, nil};
    }
    t = make(moku::slice<byte>, len(s) - 2 * percents);
    j = 0;
    {
        int i{0};
        for (i = 0; i < len(s);) {
            if ((s[i] == '%')) {
            _ident_0_:
                t[j] = unhex(s[i + 1]) << 4 | unhex(s[i + 2]);
                j++;
                i += 3;
            } else {
            _ident_1_:
                t[j] = s[i];
                j++;
                i++;
            }
        }
    }
    return {std::string(t), nil};
}
bool ishex(uint8_t c)
{
    if (('0' <= c && c <= '9')) {
    _ident_2_:
        return true;
    } else if (('a' <= c && c <= 'f')) {
    _ident_3_:
        return true;
    } else if (('A' <= c && c <= 'F')) {
    _ident_4_:
        return true;
    }
    return false;
}
uint8_t unhex(uint8_t c)
{
    if (('0' <= c && c <= '9')) {
    _ident_5_:
        return c - '0';
    } else if (('a' <= c && c <= 'f')) {
    _ident_6_:
        return c - 'a' + 10;
    } else if (('A' <= c && c <= 'F')) {
    _ident_7_:
        return c - 'A' + 10;
    }
    return 0;
}
void init();
int main()
{
    init();
    errInvalidWord = errors::New("mime: invalid RFC 2047 encoded-word");
    maxBase64Len = base64::StdEncoding->DecodedLen(maxContentLen);
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidMediaParameter = errors::New("mime: invalid media parameter");
    builtinTypesLower = std::map<std::string, std::string>{
            {".css", "text/css; charset=utf-8"},
            {".gif", "image/gif"},
            {".htm", "text/html; charset=utf-8"},
            {".html", "text/html; charset=utf-8"},
            {".jpg", "image/jpeg"},
            {".js", "application/x-javascript"},
            {".pdf", "application/pdf"},
            {".png", "image/png"},
            {".svg", "image/svg+xml"},
            {".xml", "text/xml; charset=utf-8"}};
    typeFiles = moku::slice<std::string>{"/etc/mime.types",
                                         "/etc/apache2/mime.types",
                                         "/etc/apache/mime.types"};
    _main();
    return 0;
}
void clearSyncMap(Map *m)
{
    m->Range([=](moku::interface k, moku::interface _) mutable -> bool {
        m->Delete(moku::make_iface<moku::interface>(k));
        return true;
    });
}
void setMimeTypes(std::map<std::string, std::string> lowerExt,
                  std::map<std::string, std::string> mixExt)
{
    moku::defer _defer_;

    clearSyncMap(&mimeTypes);
    clearSyncMap(&mimeTypesLower);
    clearSyncMap(&extensions);
    {
        std::string k{""};
        std::string v{""};
        for (std::tie(k, v) :
             moku::range_key_value<std::map<std::string, std::string>>(
                     lowerExt)) {
            mimeTypesLower.Store(moku::make_iface<std::string>(k),
                                 moku::make_iface<std::string>(v));
        }
    }
    {
        std::string k{""};
        std::string v{""};
        for (std::tie(k, v) :
             moku::range_key_value<std::map<std::string, std::string>>(
                     mixExt)) {
            mimeTypes.Store(moku::make_iface<std::string>(k),
                            moku::make_iface<std::string>(v));
        }
    }
    extensionsMu.Lock();
    _defer_.Push([=]() mutable { extensionsMu.Unlock(); });
    {
        std::string k{""};
        std::string v{""};
        for (std::tie(k, v) :
             moku::range_key_value<std::map<std::string, std::string>>(
                     lowerExt)) {
            std::tie(justType, _, err) = ParseMediaType(v);
            if (!moku::is_nil(err)) {
                panic(moku::make_iface<moku::error>(err));
            }
            {
                moku::interface ei{};
                bool ok{false};

                std::tie(ei, ok) =
                        extensions.Load(moku::make_iface<std::string>(k));

                if (ok) {
                    exts = moku::type_assert<moku::slice<std::string>>(ei);
                }
            }
            extensions.Store(
                    moku::make_iface<std::string>(justType),
                    moku::make_iface<moku::slice<std::string>>(
                            &{append % !s(token.Pos = 5707308)[exts k] %
                              !s(token.Pos = 0) % !s(token.Pos = 5707316)}));
        }
    }
}
void initMime()
{
    {
        std::function<void()> fn{std::nullptr};

        fn = testInitMime;

        if (!moku::is_nil(fn)) {
            fn();
        } else {
            setMimeTypes(builtinTypesLower, builtinTypesLower);
            osInitMime();
        }
    }
}
std::string TypeByExtension(std::string ext)
{
    std::vector<uint8_t> buf{};
    moku::slice<uint8_t> lower{};
    std::string s{""};
    moku::interface si{};
    constexpr int utf8RuneSelf{128};

    once.Do(initMime);
    {
        bool ok{false};
        moku::interface v{};

        std::tie(v, ok) = mimeTypes.Load(moku::make_iface<std::string>(ext));

        if (ok) {
            return moku::try_assert<std::string>(v);
        }
    }
    lower = moku::slice_expr<std::vector<uint8_t>>(buf, 0);
    {
        int i{0};
        for (i = 0; i < len(ext); i++) {
            uint8_t c{0};

            c = ext[i];
            if (c >= utf8RuneSelf) {
                std::tie(si, _) =
                        mimeTypesLower.Load(moku::make_iface<std::string>(&{
                                %
                                !s(*ast.SelectorExpr =
                                           &{0xc421a25500 0xc421a25520}) %
                                !s(token.Pos = 5709085)[ext] %
                                !s(token.Pos = 0) % !s(token.Pos = 5709089)}));
                std::tie(s, _) = moku::try_assert<std::string>(si);
                return s;
            }
            if ('A' <= c && c <= 'Z') {
                lower = append(lower, c + ('a' - 'A'));
            } else {
                lower = append(lower, c);
            }
        }
    }
    std::tie(si, _) = mimeTypesLower.Load(moku::make_iface<std::string>(
            &{string % !s(token.Pos = 5709279)[lower] % !s(token.Pos = 0) %
              !s(token.Pos = 5709285)}));
    std::tie(s, _) = moku::try_assert<std::string>(si);
    return s;
}
std::tuple<moku::slice<std::string>, moku::error> ExtensionsByType(
        std::string typ)
{
    moku::error err{};
    std::string justType{""};
    bool ok{false};
    moku::interface s{};

    std::tie(justType, _, err) = ParseMediaType(typ);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    once.Do(initMime);
    std::tie(s, ok) = extensions.Load(moku::make_iface<std::string>(justType));
    if (!ok) {
        return {nil, nil};
    }
    return {append(moku::slice<std::string>{},
                   moku::try_assert<moku::slice<std::string>>(s)),
            nil};
}
moku::error AddExtensionType(std::string ext, std::string typ)
{
    if (!strings::HasPrefix(ext, ".")) {
        return fmt::Errorf("mime: extension %q missing leading dot", ext);
    }
    once.Do(initMime);
    return setExtensionType(ext, typ);
}
moku::error setExtensionType(std::string extension, std::string mimeType)
{
    moku::defer _defer_;
    moku::error err{};
    std::string extLower{""};
    moku::slice<std::string> exts{};
    std::string justType{""};
    std::map<std::string, std::string> param{};

    std::tie(justType, param, err) = ParseMediaType(mimeType);
    if (!moku::is_nil(err)) {
        return err;
    }
    if (strings::HasPrefix(mimeType, "text/") && param["charset"] == "") {
        param["charset"] = "utf-8";
        mimeType = FormatMediaType(mimeType, param);
    }
    extLower = strings::ToLower(extension);
    mimeTypes.Store(moku::make_iface<std::string>(extension),
                    moku::make_iface<std::string>(mimeType));
    mimeTypesLower.Store(moku::make_iface<std::string>(extLower),
                         moku::make_iface<std::string>(mimeType));
    extensionsMu.Lock();
    _defer_.Push([=]() mutable { extensionsMu.Unlock(); });
    {
        moku::interface ei{};
        bool ok{false};

        std::tie(ei, ok) =
                extensions.Load(moku::make_iface<std::string>(justType));

        if (ok) {
            exts = moku::type_assert<moku::slice<std::string>>(ei);
        }
    }
    {
        std::string v{""};
        for (v : moku::range_value<moku::slice<std::string>>(exts)) {
            if (v == extLower) {
                return nil;
            }
        }
    }
    extensions.Store(moku::make_iface<std::string>(justType),
                     moku::make_iface<moku::slice<std::string>>(
                             &{append % !s(token.Pos = 5710857)[exts extLower] %
                               !s(token.Pos = 0) % !s(token.Pos = 5710872)}));
    return nil;
}
void init();
int main()
{
    init();
    errInvalidWord = errors::New("mime: invalid RFC 2047 encoded-word");
    maxBase64Len = base64::StdEncoding->DecodedLen(maxContentLen);
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidMediaParameter = errors::New("mime: invalid media parameter");
    builtinTypesLower = std::map<std::string, std::string>{
            {".css", "text/css; charset=utf-8"},
            {".gif", "image/gif"},
            {".htm", "text/html; charset=utf-8"},
            {".html", "text/html; charset=utf-8"},
            {".jpg", "image/jpeg"},
            {".js", "application/x-javascript"},
            {".pdf", "application/pdf"},
            {".png", "image/png"},
            {".svg", "image/svg+xml"},
            {".xml", "text/xml; charset=utf-8"}};
    typeFiles = moku::slice<std::string>{"/etc/mime.types",
                                         "/etc/apache2/mime.types",
                                         "/etc/apache/mime.types"};
    _main();
    return 0;
}
void init()
{
    osInitMime = initMimeUnix;
}
void loadMimeFile(std::string filename)
{
    moku::defer _defer_;
    moku::error err{};
    File *f{std::nullptr};
    Scanner *scanner{std::nullptr};

    std::tie(f, err) = os::Open(filename);
    if (!moku::is_nil(err)) {
        return;
    }
    _defer_.Push([=]() mutable { f->Close(); });
    scanner = bufio::NewScanner(f);
    while (scanner->Scan()) {
        moku::slice<std::string> fields{};
        std::string mimeType{""};

        fields = strings::Fields(scanner->Text());
        if (len(fields) <= 1 || fields[0][0] == '#') {
            continue;
        }
        mimeType = fields[0];
        {
            std::string ext{""};
            for (ext : moku::range_value<moku::slice<std::string>>(
                         moku::slice_expr<moku::slice<std::string>>(fields,
                                                                    1))) {
                if (ext[0] == '#') {
                    break;
                }
                setExtensionType("." + ext, mimeType);
            }
        }
    }
    {
        moku::error err{};

        err = scanner->Err();

        if (!moku::is_nil(err)) {
            panic(moku::make_iface<moku::error>(err));
        }
    }
}
void initMimeUnix()
{
    {
        std::string filename{""};
        for (filename :
             moku::range_value<moku::slice<std::string>>(typeFiles)) {
            loadMimeFile(filename);
        }
    }
}
std::map<std::string, std::string> initMimeForTests()
{
    typeFiles = moku::slice<std::string>{"testdata/test.types"};
    return std::map<std::string, std::string>{
            {".T1", "application/test"},
            {".t2", "text/test; charset=utf-8"},
            {".png", "image/png"}};
}
void init();
int main()
{
    init();
    errInvalidWord = errors::New("mime: invalid RFC 2047 encoded-word");
    maxBase64Len = base64::StdEncoding->DecodedLen(maxContentLen);
    bufPool = sync::Pool{{New, [=]() mutable -> moku::interface {
                              return new (bytes::Buffer);
                          }}};
    ErrInvalidMediaParameter = errors::New("mime: invalid media parameter");
    builtinTypesLower = std::map<std::string, std::string>{
            {".css", "text/css; charset=utf-8"},
            {".gif", "image/gif"},
            {".htm", "text/html; charset=utf-8"},
            {".html", "text/html; charset=utf-8"},
            {".jpg", "image/jpeg"},
            {".js", "application/x-javascript"},
            {".pdf", "application/pdf"},
            {".png", "image/png"},
            {".svg", "image/svg+xml"},
            {".xml", "text/xml; charset=utf-8"}};
    typeFiles = moku::slice<std::string>{"/etc/mime.types",
                                         "/etc/apache2/mime.types",
                                         "/etc/apache/mime.types"};
    _main();
    return 0;
}
