std::tuple<int64_t, bool> parseInt(moku::slice<uint8_t> bytes)
{
    bool neg{false};
    bool ok{false};
    int64_t v{0};

    if (len(bytes) == 0) {
        return {0, false};
    }
    if (bytes[0] == '-') {
        neg = true;
        bytes = moku::slice_expr<moku::slice<uint8_t>>(bytes, 1);
    }
    {
        uint8_t c{0};
        for (c : moku::range_value<moku::slice<uint8_t>>(bytes)) {
            if (c >= '0' && c <= '9') {
                v = (10 * v) + int64_t(c - '0');
            } else {
                return {0, false};
            }
        }
    }
    if (neg) {
        return {-v, true};
    } else {
        return {v, true};
    }
}
void init();
int main()
{
    init();
    backslashCharEscapeTable = std::vector<byte>{{'"', '"'},
                                                 {'\\', '\\'},
                                                 {'/', '/'},
                                                 {'b', '\b'},
                                                 {'f', '\f'},
                                                 {'n', '\n'},
                                                 {'r', '\r'},
                                                 {'t', '\t'}};
    KeyPathNotFoundError = errors::New("Key path not found");
    UnknownValueTypeError = errors::New("Unknown value type");
    MalformedJsonError = errors::New("Malformed JSON error");
    MalformedStringError =
            errors::New("Value is string, but can't find closing '\"' symbol");
    MalformedArrayError =
            errors::New("Value is array, but can't find closing ']' symbol");
    MalformedObjectError = errors::New(
            "Value looks like object, but can't find closing '}' symbol");
    MalformedValueError = errors::New(
            "Value looks like Number/Boolean/None, but can't find its end: ',' "
            "or '}' symbol");
    MalformedStringEscapeError =
            errors::New("Encountered an invalid escape sequence in a string");
    trueLiteral = moku::slice<byte>("true");
    falseLiteral = moku::slice<byte>("false");
    nullLiteral = moku::slice<byte>("null");
    _main();
    return 0;
}
bool equalStr(moku::slice<uint8_t> *b, std::string s)
{
    return *(*std::string)(unsafe::Pointer(b)) == s;
}
std::tuple<double, moku::error> parseFloat(moku::slice<uint8_t> *b)
{
    return strconv::ParseFloat(*(*std::string)(unsafe::Pointer(b)), 64);
}
std::string bytesToString(moku::slice<uint8_t> *b)
{
    return *(*std::string)(unsafe::Pointer(b));
}
moku::slice<uint8_t> StringToBytes(std::string s)
{
    SliceHeader bh{};
    StringHeader *sh{std::nullptr};

    sh = (*reflect::StringHeader)(unsafe::Pointer(&s));
    bh = reflect::SliceHeader{Data : sh->Data, Len : sh->Len, Cap : sh->Len};
    return *(*moku::slice<byte>)(unsafe::Pointer(&bh));
}
void init();
int main()
{
    init();
    backslashCharEscapeTable = std::vector<byte>{{'"', '"'},
                                                 {'\\', '\\'},
                                                 {'/', '/'},
                                                 {'b', '\b'},
                                                 {'f', '\f'},
                                                 {'n', '\n'},
                                                 {'r', '\r'},
                                                 {'t', '\t'}};
    KeyPathNotFoundError = errors::New("Key path not found");
    UnknownValueTypeError = errors::New("Unknown value type");
    MalformedJsonError = errors::New("Malformed JSON error");
    MalformedStringError =
            errors::New("Value is string, but can't find closing '\"' symbol");
    MalformedArrayError =
            errors::New("Value is array, but can't find closing ']' symbol");
    MalformedObjectError = errors::New(
            "Value looks like object, but can't find closing '}' symbol");
    MalformedValueError = errors::New(
            "Value looks like Number/Boolean/None, but can't find its end: ',' "
            "or '}' symbol");
    MalformedStringEscapeError =
            errors::New("Encountered an invalid escape sequence in a string");
    trueLiteral = moku::slice<byte>("true");
    falseLiteral = moku::slice<byte>("false");
    nullLiteral = moku::slice<byte>("null");
    _main();
    return 0;
}
int32_t combineUTF16Surrogates(int32_t high, int32_t low)
{
    return supplementalPlanesOffset + (high - highSurrogateOffset)
           << 10 + (low - lowSurrogateOffset);
}
int h2I(uint8_t c)
{
    if ((c >= '0' && c <= '9')) {
    _ident_0_:
        return int(c - '0');
    } else if ((c >= 'A' && c <= 'F')) {
    _ident_1_:
        return int(c - 'A' + 10);
    } else if ((c >= 'a' && c <= 'f')) {
    _ident_2_:
        return int(c - 'a' + 10);
    }
    return badHex;
}
std::tuple<int32_t, bool> decodeSingleUnicodeEscape(moku::slice<uint8_t> in)
{
    int h1{0};
    int h2{0};
    int h3{0};
    int h4{0};

    if (len(in) < 6) {
        return {utf8::RuneError, false};
    }
    std::tie(h1, h2, h3, h4) = std::tuple<int, int, int, int>(
            h2I(in[2]), h2I(in[3]), h2I(in[4]), h2I(in[5]));
    if (h1 == badHex || h2 == badHex || h3 == badHex || h4 == badHex) {
        return {utf8::RuneError, false};
    }
    return {rune(h1 << 12 + h2 << 8 + h3 << 4 + h4), true};
}
bool isUTF16EncodedRune(int32_t r)
{
    return highSurrogateOffset <= r &&
           r <= basicMultilingualPlaneReservedOffset;
}
std::tuple<int32_t, int> decodeUnicodeEscape(moku::slice<uint8_t> in)
{
    {
        bool ok{false};
        int32_t r{0};

        std::tie(r, ok) = decodeSingleUnicodeEscape(in);

        if (!ok) {
            return {utf8::RuneError, -1};
        } else {
            if (r <= basicMultilingualPlaneOffset && !isUTF16EncodedRune(r)) {
                return {r, 6};
            } else {
                {
                    bool ok{false};
                    int32_t r2{0};

                    std::tie(r2, ok) = decodeSingleUnicodeEscape(
                            moku::slice_expr<moku::slice<uint8_t>>(in, 6));

                    if (!ok) {
                        return {utf8::RuneError, -1};
                    } else {
                        if (r2 < lowSurrogateOffset) {
                            return {utf8::RuneError, -1};
                        } else {
                            return {combineUTF16Surrogates(r, r2), 12};
                        }
                    }
                }
            }
        }
    }
}
std::tuple<int, int> unescapeToUTF8(moku::slice<uint8_t> in,
                                    moku::slice<uint8_t> out)
{
    int inLen{0};
    int outLen{0};

    if (len(in) < 2 || in[0] != '\\') {
        return {-1, -1};
    }
    {
        uint8_t e{0};
        e = in[1];
        if ((e == '"') || (e == '\\') || (e == '/') || (e == 'b') ||
            (e == 'f') || (e == 'n') || (e == 'r') || (e == 't')) {
        _ident_3_:
            out[0] = backslashCharEscapeTable[e];
            return {2, 1};
        } else if ((e == 'u')) {
        _ident_4_ : {
            int inLen{0};
            int32_t r{0};

            std::tie(r, inLen) = decodeUnicodeEscape(in);

            if (inLen == -1) {
                return {-1, -1};
            } else {
                outLen = utf8::EncodeRune(out, r);
                return {inLen, outLen};
            }
        }
        }
    }
    return {-1, -1};
}
std::tuple<moku::slice<uint8_t>, moku::error> Unescape(moku::slice<uint8_t> in,
                                                       moku::slice<uint8_t> out)
{
    moku::slice<uint8_t> buf{};
    int firstBackslash{0};

    firstBackslash = bytes::IndexByte(in, '\\');
    if (firstBackslash == -1) {
        return {in, nil};
    }
    if (cap(out) < len(in)) {
        out = make(moku::slice<byte>, len(in));
    } else {
        out = moku::slice_expr<moku::slice<uint8_t>>(out, 0, len(in));
    }
    copy(out, moku::slice_expr<moku::slice<uint8_t>>(in, firstBackslash));
    in = moku::slice_expr<moku::slice<uint8_t>>(in, firstBackslash);
    buf = moku::slice_expr<moku::slice<uint8_t>>(out, firstBackslash);
    while (len(in) > 0) {
        int bufLen{0};
        int inLen{0};
        int nextBackslash{0};

        std::tie(inLen, bufLen) = unescapeToUTF8(in, buf);
        if (inLen == -1) {
            return {nil, MalformedStringEscapeError};
        }
        in = moku::slice_expr<moku::slice<uint8_t>>(in, inLen);
        buf = moku::slice_expr<moku::slice<uint8_t>>(buf, bufLen);
        nextBackslash = bytes::IndexByte(in, '\\');
        if (nextBackslash == -1) {
            copy(buf, in);
            buf = moku::slice_expr<moku::slice<uint8_t>>(buf, len(in));
            break;
        } else {
            copy(buf,
                 moku::slice_expr<moku::slice<uint8_t>>(in, nextBackslash));
            buf = moku::slice_expr<moku::slice<uint8_t>>(buf, nextBackslash);
            in = moku::slice_expr<moku::slice<uint8_t>>(in, nextBackslash);
        }
    }
    return {moku::slice_expr<moku::slice<uint8_t>>(out, len(out) - len(buf)),
            nil};
}
void init();
int main()
{
    init();
    backslashCharEscapeTable = std::vector<byte>{{'"', '"'},
                                                 {'\\', '\\'},
                                                 {'/', '/'},
                                                 {'b', '\b'},
                                                 {'f', '\f'},
                                                 {'n', '\n'},
                                                 {'r', '\r'},
                                                 {'t', '\t'}};
    KeyPathNotFoundError = errors::New("Key path not found");
    UnknownValueTypeError = errors::New("Unknown value type");
    MalformedJsonError = errors::New("Malformed JSON error");
    MalformedStringError =
            errors::New("Value is string, but can't find closing '\"' symbol");
    MalformedArrayError =
            errors::New("Value is array, but can't find closing ']' symbol");
    MalformedObjectError = errors::New(
            "Value looks like object, but can't find closing '}' symbol");
    MalformedValueError = errors::New(
            "Value looks like Number/Boolean/None, but can't find its end: ',' "
            "or '}' symbol");
    MalformedStringEscapeError =
            errors::New("Encountered an invalid escape sequence in a string");
    trueLiteral = moku::slice<byte>("true");
    falseLiteral = moku::slice<byte>("false");
    nullLiteral = moku::slice<byte>("null");
    _main();
    return 0;
}
int tokenEnd(moku::slice<uint8_t> data)
{
    {
        uint8_t c{0};
        int i{0};
        for (std::tie(i, c) :
             moku::range_key_value<moku::slice<uint8_t>>(data)) {
            if ((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t') ||
                (c == ',') || (c == '}') || (c == ']')) {
            _ident_0_:
                return i;
            }
        }
    }
    return len(data);
}
int findTokenStart(moku::slice<uint8_t> data, uint8_t token)
{
    {
        int i{0};
        for (i = len(data) - 1; i >= 0; i--) {
            if ((data[i] == token)) {
            _ident_1_:
                return i;
            } else if ((data[i] == '[') || (data[i] == '{')) {
            _ident_2_:
                return 0;
            }
        }
    }
    return 0;
}
std::tuple<int, moku::error> findKeyStart(moku::slice<uint8_t> data,
                                          std::string key)
{
    int i{0};
    int ln{0};
    std::vector<uint8_t> stackbuf{};

    i = 0;
    ln = len(data);
    if (ln > 0 && (data[0] == '{' || data[0] == '[')) {
        i = 1;
    }
    {
        moku::error err{};
        moku::slice<uint8_t> ku{}; /* escapes */

        std::tie(ku, err) =
                Unescape(StringToBytes(key),
                         moku::slice_expr<std::vector<uint8_t>>(stackbuf));

        if (moku::is_nil(err)) {
            key = bytesToString(&ku);
        }
    }
    while (i < ln) {
        if ((data[i] == '"')) {
        _ident_3_:
            i++;
            keyBegin = i;
            std::tie(strEnd, keyEscaped) =
                    stringEnd(moku::slice_expr<moku::slice<uint8_t>>(data, i));
            if (strEnd == -1) {
                break;
            }
            i += strEnd;
            keyEnd = i - 1;
            valueOffset =
                    nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, i));
            if (valueOffset == -1) {
                break;
            }
            i += valueOffset;
            k = moku::slice_expr<moku::slice<uint8_t>>(data, keyBegin, keyEnd);
            if (keyEscaped) {
                {
                    moku::error err{};
                    moku::slice<uint8_t> ku{};

                    std::tie(ku, err) = Unescape(
                            k,
                            moku::slice_expr<std::vector<uint8_t>>(stackbuf));

                    if (!moku::is_nil(err)) {
                        break;
                    } else {
                        k = ku;
                    }
                }
            }
            if (data[i] == ':' && len(key) == len(k) &&
                bytesToString(&k) == key) {
                return {keyBegin - 1, nil};
            }
        } else if ((data[i] == '[')) {
        _ident_4_:
            i = blockEnd(moku::slice_expr<moku::slice<uint8_t>>(data, i),
                         data[i],
                         ']') +
                i;
        } else if ((data[i] == '{')) {
        _ident_5_:
            i = blockEnd(moku::slice_expr<moku::slice<uint8_t>>(data, i),
                         data[i],
                         '}') +
                i;
        }
        i++;
    }
    return {-1, KeyPathNotFoundError};
}
int tokenStart(moku::slice<uint8_t> data)
{
    {
        int i{0};
        for (i = len(data) - 1; i >= 0; i--) {
            if ((data[i] == '\n') || (data[i] == '\r') || (data[i] == '\t') ||
                (data[i] == ',') || (data[i] == '{') || (data[i] == '[')) {
            _ident_6_:
                return i;
            }
        }
    }
    return 0;
}
int nextToken(moku::slice<uint8_t> data)
{
    {
        uint8_t c{0};
        int i{0};
        for (std::tie(i, c) :
             moku::range_key_value<moku::slice<uint8_t>>(data)) {
            if ((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t')) {
            _ident_7_:
                continue;
            } else {
            _ident_8_:
                return i;
            }
        }
    }
    return -1;
}
int lastToken(moku::slice<uint8_t> data)
{
    {
        int i{0};
        for (i = len(data) - 1; i >= 0; i--) {
            if ((data[i] == ' ') || (data[i] == '\n') || (data[i] == '\r') ||
                (data[i] == '\t')) {
            _ident_9_:
                continue;
            } else {
            _ident_10_:
                return i;
            }
        }
    }
    return -1;
}
std::tuple<int, bool> stringEnd(moku::slice<uint8_t> data)
{
    bool escaped{false};

    escaped = false;
    {
        uint8_t c{0};
        int i{0};
        for (std::tie(i, c) :
             moku::range_key_value<moku::slice<uint8_t>>(data)) {
            if (c == '"') {
                if (!escaped) {
                    return {i + 1, false};
                } else {
                    j = i - 1;
                    while (true) {
                        if (j < 0 || data[j] != '\\') {
                            return {i + 1, true};
                        }
                        j--;
                        if (j < 0 || data[j] != '\\') {
                            break;
                        }
                        j--;
                    }
                }
            } else {
                if (c == '\\') {
                    escaped = true;
                }
            }
        }
    }
    return {-1, escaped};
}
int blockEnd(moku::slice<uint8_t> data, uint8_t openSym, uint8_t closeSym)
{
    int i{0};
    int level{0};
    int ln{0};

    level = 0;
    i = 0;
    ln = len(data);
    while (i < ln) {
        if ((data[i] == '"')) {
        _ident_11_:
            std::tie(se, _) = stringEnd(
                    moku::slice_expr<moku::slice<uint8_t>>(data, i + 1));
            if (se == -1) {
                return -1;
            }
            i += se;
        } else if ((data[i] == openSym)) {
        _ident_12_:
            level++;
        } else if ((data[i] == closeSym)) {
        _ident_13_:
            level--;
            if (level == 0) {
                return i + 1;
            }
        }
        i++;
    }
    return -1;
}
int searchKeys(moku::slice<uint8_t> data, moku::slice<std::string> keys)
{
    int i{0};
    int keyLevel{0};
    int level{0};
    int lk{0};
    int ln{0};
    std::vector<uint8_t> stackbuf{};

    keyLevel = 0;
    level = 0;
    i = 0;
    ln = len(data);
    lk = len(keys);
    if (lk == 0) {
        return 0;
    }
    while (i < ln) {
        if ((data[i] == '"')) {
        _ident_14_:
            i++;
            keyBegin = i;
            std::tie(strEnd, keyEscaped) =
                    stringEnd(moku::slice_expr<moku::slice<uint8_t>>(data, i));
            if (strEnd == -1) {
                return -1;
            }
            i += strEnd;
            keyEnd = i - 1;
            valueOffset =
                    nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, i));
            if (valueOffset == -1) {
                return -1;
            }
            i += valueOffset;
            if (data[i] == ':' && keyLevel == level - 1) {
                key = moku::slice_expr<moku::slice<uint8_t>>(
                        data, keyBegin, keyEnd);
                if (!keyEscaped) {
                    keyUnesc = key;
                } else {
                    {
                        moku::error err{};
                        moku::slice<uint8_t> ku{};

                        std::tie(ku, err) =
                                Unescape(key,
                                         moku::slice_expr<std::vector<uint8_t>>(
                                                 stackbuf));

                        if (!moku::is_nil(err)) {
                            return -1;
                        } else {
                            keyUnesc = ku;
                        }
                    }
                }
                if (equalStr(&keyUnesc, keys[level - 1])) {
                    keyLevel++;
                    if (keyLevel == lk) {
                        return i + 1;
                    }
                }
            } else {
                i--;
            }
        } else if ((data[i] == '{')) {
        _ident_15_:
            level++;
        } else if ((data[i] == '}')) {
        _ident_16_:
            level--;
            if (level == keyLevel) {
                keyLevel--;
            }
        } else if ((data[i] == '[')) {
        _ident_17_:
            if (keyLevel == level && keys[level][0] == '[') {
                std::tie(aIdx, err) =
                        strconv::Atoi(moku::slice_expr<std::string>(
                                keys[level], 1, len(keys[level]) - 1));
                if (!moku::is_nil(err)) {
                    return -1;
                }
                ArrayEach(moku::slice_expr<moku::slice<uint8_t>>(data, i),
                          [=](moku::slice<uint8_t> value,
                              ValueType dataType,
                              int offset,
                              moku::error err) mutable -> void {
                              if (curIdx == aIdx) {
                                  valueFound = value;
                                  valueOffset = offset;
                                  if (dataType == String) {
                                      valueOffset = valueOffset - 2;
                                      valueFound = moku::slice_expr<
                                              moku::slice<uint8_t>>(
                                              data,
                                              curI + valueOffset,
                                              curI + valueOffset + len(value) +
                                                      2);
                                  }
                              }
                              curIdx += 1;
                          });
                if (moku::is_nil(valueFound)) {
                    return -1;
                } else {
                    subIndex = searchKeys(
                            valueFound,
                            moku::slice_expr<moku::slice<std::string>>(
                                    keys, level + 1));
                    if (subIndex < 0) {
                        return -1;
                    }
                    return i + valueOffset + subIndex;
                }
            } else {
                {
                    int arraySkip{0};

                    arraySkip = blockEnd(
                            moku::slice_expr<moku::slice<uint8_t>>(data, i),
                            '[',
                            ']');

                    if (arraySkip == -1) {
                        return -1;
                    } else {
                        i += arraySkip - 1;
                    }
                }
            }
        }
        i++;
    }
    return -1;
}
void init()
{
    {
        int i{0};
        for (i = 0; i < 63; i++) {
            bitwiseFlags =
                    append(bitwiseFlags, int64_t(math::Pow(2, double(i))));
        }
    }
}
bool sameTree(moku::slice<std::string> p1, moku::slice<std::string> p2)
{
    int minLen{0};

    minLen = len(p1);
    if (len(p2) < minLen) {
        minLen = len(p2);
    }
    {
        std::string p_1{""};
        int pi_1{0};
        for (std::tie(pi_1, p_1) :
             moku::range_key_value<moku::slice<std::string>>(
                     moku::slice_expr<moku::slice<std::string>>(p1, minLen))) {
            if (p2[pi_1] != p_1) {
                return false;
            }
        }
    }
    return true;
}
int EachKey(
        moku::slice<uint8_t> data,
        std::function<void(int, moku::slice<uint8_t>, ValueType, moku::error)>
                cb,
        moku::slice<moku::slice<std::string>> paths)
{
    int i{0};
    int level{0};
    int ln{0};
    int maxPath{0};
    int64_t pathFlags{0};
    moku::slice<std::string> pathsBuf{};
    int pathsMatched{0};
    std::vector<uint8_t> stackbuf{};

    ln = len(data);
    {
        moku::slice<std::string> p{};
        for (p :
             moku::range_value<moku::slice<moku::slice<std::string>>>(paths)) {
            if (len(p) > maxPath) {
                maxPath = len(p);
            }
        }
    }
    pathsBuf = make(moku::slice<std::string>, maxPath);
    while (i < ln) {
        if ((data[i] == '"')) {
        _ident_18_:
            i++;
            keyBegin = i;
            std::tie(strEnd, keyEscaped) =
                    stringEnd(moku::slice_expr<moku::slice<uint8_t>>(data, i));
            if (strEnd == -1) {
                return -1;
            }
            i += strEnd;
            keyEnd = i - 1;
            valueOffset =
                    nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, i));
            if (valueOffset == -1) {
                return -1;
            }
            i += valueOffset;
            if (data[i] == ':') {
                match = -1;
                key = moku::slice_expr<moku::slice<uint8_t>>(
                        data, keyBegin, keyEnd);
                if (!keyEscaped) {
                    keyUnesc = key;
                } else {
                    {
                        moku::error err{};
                        moku::slice<uint8_t> ku{};

                        std::tie(ku, err) =
                                Unescape(key,
                                         moku::slice_expr<std::vector<uint8_t>>(
                                                 stackbuf));

                        if (!moku::is_nil(err)) {
                            return -1;
                        } else {
                            keyUnesc = ku;
                        }
                    }
                }
                if (maxPath >= level) {
                    if (level < 1) {
                        cb(-1, nil, Unknown, MalformedJsonError);
                        return -1;
                    }
                    pathsBuf[level - 1] = bytesToString(&keyUnesc);
                    {
                        moku::slice<std::string> p{};
                        int pi{0};
                        for (std::tie(pi, p) : moku::range_key_value<
                                     moku::slice<moku::slice<std::string>>>(
                                     paths)) {
                            if (len(p) != level ||
                                pathFlags & bitwiseFlags[pi + 1] != 0 ||
                                !equalStr(&keyUnesc, p[level - 1]) ||
                                !sameTree(p,
                                          moku::slice_expr<
                                                  moku::slice<std::string>>(
                                                  pathsBuf, level))) {
                                continue;
                            }
                            match = pi;
                            i++;
                            pathsMatched++;
                            pathFlags |= bitwiseFlags[pi + 1];
                            std::tie(v, dt, of, e) =
                                    Get(moku::slice_expr<moku::slice<uint8_t>>(
                                            data, i));
                            cb(pi, v, dt, e);
                            if (of != -1) {
                                i += of;
                            }
                            if (pathsMatched == len(paths)) {
                                break;
                            }
                        }
                    }
                    if (pathsMatched == len(paths)) {
                        return i;
                    }
                }
                if (match == -1) {
                    tokenOffset =
                            nextToken(moku::slice_expr<moku::slice<uint8_t>>(
                                    data, i + 1));
                    i += tokenOffset;
                    if (data[i] == '{') {
                        blockSkip = blockEnd(
                                moku::slice_expr<moku::slice<uint8_t>>(data, i),
                                '{',
                                '}');
                        i += blockSkip + 1;
                    }
                }
                if ((data[i] == '{') || (data[i] == '}') || (data[i] == '[') ||
                    (data[i] == '"')) {
                _ident_23_:
                    i--;
                }
            } else {
                i--;
            }
        } else if ((data[i] == '{')) {
        _ident_19_:
            level++;
        } else if ((data[i] == '}')) {
        _ident_20_:
            level--;
        } else if ((data[i] == '[')) {
        _ident_21_:
            if (level < 0) {
                cb(-1, nil, Unknown, MalformedJsonError);
                return -1;
            }
            {
                moku::slice<std::string> p{};
                int pi{0};
                for (std::tie(pi, p) : moku::range_key_value<
                             moku::slice<moku::slice<std::string>>>(paths)) {
                    if (len(p) < level + 1 ||
                        pathFlags & bitwiseFlags[pi + 1] != 0 ||
                        p[level][0] != '[' ||
                        !sameTree(p,
                                  moku::slice_expr<moku::slice<std::string>>(
                                          pathsBuf, level))) {
                        continue;
                    }
                    std::tie(aIdx, _) =
                            strconv::Atoi(moku::slice_expr<std::string>(
                                    p[level], 1, len(p[level]) - 1));
                    arrIdxFlags |= bitwiseFlags[aIdx + 1];
                    pIdxFlags |= bitwiseFlags[pi + 1];
                }
            }
            if (arrIdxFlags > 0) {
                level++;
                std::tie(arrOff, _) = ArrayEach(
                        moku::slice_expr<moku::slice<uint8_t>>(data, i),
                        [=](moku::slice<uint8_t> value,
                            ValueType dataType,
                            int offset,
                            moku::error err) mutable -> void {
                            if (arrIdxFlags & bitwiseFlags[curIdx + 1] != 0) {
                                {
                                    moku::slice<std::string> p{};
                                    int pi{0};
                                    for (std::tie(pi, p) :
                                         moku::range_key_value<moku::slice<
                                                 moku::slice<std::string>>>(
                                                 paths)) {
                                        if (pIdxFlags &
                                            bitwiseFlags[pi + 1] != 0) {
                                            std::tie(aIdx, _) = strconv::Atoi(
                                                    moku::slice_expr<
                                                            std::string>(
                                                            p[level - 1],
                                                            1,
                                                            len(p[level - 1]) -
                                                                    1));
                                            if (curIdx == aIdx) {
                                                of = searchKeys(
                                                        value,
                                                        moku::slice_expr<moku::slice<
                                                                std::string>>(
                                                                p, level));
                                                pathsMatched++;
                                                pathFlags |=
                                                        bitwiseFlags[pi + 1];
                                                if (of != -1) {
                                                    std::tie(v, dt, _, e) = Get(
                                                            moku::slice_expr<
                                                                    moku::slice<
                                                                            uint8_t>>(
                                                                    value, of));
                                                    cb(pi, v, dt, e);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            curIdx += 1;
                        });
                if (pathsMatched == len(paths)) {
                    return i;
                }
                i += arrOff - 1;
            } else {
                {
                    int arraySkip{0};

                    arraySkip = blockEnd(
                            moku::slice_expr<moku::slice<uint8_t>>(data, i),
                            '[',
                            ']');

                    if (arraySkip == -1) {
                        return -1;
                    } else {
                        i += arraySkip - 1;
                    }
                }
            }
        } else if ((data[i] == ']')) {
        _ident_22_:
            level--;
        }
        i++;
    }
    return -1;
}
std::string ValueType::_StringByValue()
{
    if ((this == NotExist)) {
    _ident_24_:
        return "non-existent";
    } else if ((this == String)) {
    _ident_25_:
        return "string";
    } else if ((this == Number)) {
    _ident_26_:
        return "number";
    } else if ((this == Object)) {
    _ident_27_:
        return "object";
    } else if ((this == Array)) {
    _ident_28_:
        return "array";
    } else if ((this == Boolean)) {
    _ident_29_:
        return "boolean";
    } else if ((this == Null)) {
    _ident_30_:
        return "null";
    } else {
    _ident_31_:
        return "unknown";
    }
}
moku::slice<uint8_t> createInsertComponent(moku::slice<std::string> keys,
                                           moku::slice<uint8_t> setValue,
                                           bool comma,
                                           bool object)
{
    Buffer buffer{};
    bool isIndex{false};

    isIndex = std::string(keys[0][0]) == "[";
    if (comma) {
        buffer.WriteString(",");
    }
    if (isIndex) {
        buffer.WriteString("[");
    } else {
        if (object) {
            buffer.WriteString("{");
        }
        buffer.WriteString("\"");
        buffer.WriteString(keys[0]);
        buffer.WriteString("\":");
    }
    {
        int i{0};
        for (i = 1; i < len(keys); i++) {
            if (std::string(keys[i][0]) == "[") {
                buffer.WriteString("[");
            } else {
                buffer.WriteString("{\"");
                buffer.WriteString(keys[i]);
                buffer.WriteString("\":");
            }
        }
    }
    buffer.Write(setValue);
    {
        int i{0};
        for (i = len(keys) - 1; i > 0; i--) {
            if (std::string(keys[i][0]) == "[") {
                buffer.WriteString("]");
            } else {
                buffer.WriteString("}");
            }
        }
    }
    if (isIndex) {
        buffer.WriteString("]");
    }
    if (object && !isIndex) {
        buffer.WriteString("}");
    }
    return buffer.Bytes();
}
moku::slice<uint8_t> Delete(moku::slice<uint8_t> data,
                            moku::slice<std::string> keys)
{
    bool array{false};
    int endOffset{0};
    moku::error err{};
    int keyOffset{0};
    int lk{0};
    int newOffset{0};
    int prevTok{0};
    moku::slice<uint8_t> remainedValue{};
    int startOffset{0};

    lk = len(keys);
    if (lk == 0) {
        return moku::slice_expr<moku::slice<uint8_t>>(data, 0);
    }
    array = false;
    if (len(keys[lk - 1]) > 0 && std::string(keys[lk - 1][0]) == "[") {
        array = true;
    }
    endOffset = len(data);
    if (!array) {
        if (len(keys) > 1) {
            std::tie(_, _, startOffset, endOffset, err) = internalGet(
                    data,
                    moku::slice_expr<moku::slice<std::string>>(keys, lk - 1));
            if (err == KeyPathNotFoundError) {
                return data;
            }
        }
        std::tie(keyOffset, err) =
                findKeyStart(moku::slice_expr<moku::slice<uint8_t>>(
                                     data, startOffset, endOffset),
                             keys[lk - 1]);
        if (err == KeyPathNotFoundError) {
            return data;
        }
        keyOffset += startOffset;
        std::tie(_, _, _, subEndOffset, _) =
                internalGet(moku::slice_expr<moku::slice<uint8_t>>(
                                    data, startOffset, endOffset),
                            keys[lk - 1]);
        endOffset = startOffset + subEndOffset;
        tokEnd = tokenEnd(
                moku::slice_expr<moku::slice<uint8_t>>(data, endOffset));
        tokStart = findTokenStart(
                moku::slice_expr<moku::slice<uint8_t>>(data, keyOffset),
                ","[0]);
        if (data[endOffset + tokEnd] == ","[0]) {
            endOffset += tokEnd + 1;
        } else {
            if (data[endOffset + tokEnd] == " "[0] &&
                len(data) > endOffset + tokEnd + 1 &&
                data[endOffset + tokEnd + 1] == ","[0]) {
                endOffset += tokEnd + 2;
            } else {
                if (data[endOffset + tokEnd] == "}"[0] &&
                    data[tokStart] == ","[0]) {
                    keyOffset = tokStart;
                }
            }
        }
    } else {
        std::tie(_, _, keyOffset, endOffset, err) = internalGet(data, keys);
        if (err == KeyPathNotFoundError) {
            return data;
        }
        tokEnd = tokenEnd(
                moku::slice_expr<moku::slice<uint8_t>>(data, endOffset));
        tokStart = findTokenStart(
                moku::slice_expr<moku::slice<uint8_t>>(data, keyOffset),
                ","[0]);
        if (data[endOffset + tokEnd] == ","[0]) {
            endOffset += tokEnd + 1;
        } else {
            if (data[endOffset + tokEnd] == "]"[0] &&
                data[tokStart] == ","[0]) {
                keyOffset = tokStart;
            }
        }
    }
    prevTok =
            lastToken(moku::slice_expr<moku::slice<uint8_t>>(data, keyOffset));
    remainedValue = moku::slice_expr<moku::slice<uint8_t>>(data, endOffset);
    if (nextToken(remainedValue) > -1 &&
        remainedValue[nextToken(remainedValue)] == '}' &&
        data[prevTok] == ',') {
        newOffset = prevTok;
    } else {
        newOffset = prevTok + 1;
    }
    data = append(moku::slice_expr<moku::slice<uint8_t>>(data, newOffset),
                  moku::slice_expr<moku::slice<uint8_t>>(data, endOffset));
    return data;
}
std::tuple<moku::slice<uint8_t>, moku::error> Set(moku::slice<uint8_t> data,
                                                  moku::slice<uint8_t> setValue,
                                                  moku::slice<std::string> keys)
{
    int endOffset{0};
    moku::error err{};
    int startOffset{0};
    moku::slice<uint8_t> value{};

    if (len(keys) == 0) {
        return {nil, KeyPathNotFoundError};
    }
    std::tie(_, _, startOffset, endOffset, err) = internalGet(data, keys);
    if (!moku::is_nil(err)) {
        if (err != KeyPathNotFoundError) {
            return {nil, err};
        }
        {
            int i{0};
            for (i : moku::range_key<moku::slice<std::string>>(keys)) {
                std::tie(_, _, start, end, sErr) =
                        internalGet(data,
                                    moku::slice_expr<moku::slice<std::string>>(
                                            keys, i + 1));
                if (!moku::is_nil(sErr)) {
                    break;
                } else {
                    endOffset = end;
                    startOffset = start;
                    depth++;
                }
            }
        }
        comma = true;
        object = false;
        if (endOffset == -1) {
            firstToken = nextToken(data);
            if (len(data) == 0 || data[firstToken] != '{') {
                return {nil, KeyPathNotFoundError};
            }
            secondToken = firstToken + 1 +
                          nextToken(moku::slice_expr<moku::slice<uint8_t>>(
                                  data, firstToken + 1));
            if (data[secondToken] == '}') {
                comma = false;
            }
            endOffset = lastToken(data);
        }
        depthOffset = endOffset;
        if (depth != 0) {
            if (data[startOffset] == '{' &&
                data[startOffset + 1 +
                     nextToken(moku::slice_expr<moku::slice<uint8_t>>(
                             data, startOffset + 1))] != '}') {
                depthOffset--;
                startOffset = depthOffset;
            } else {
                comma = false;
                object = true;
            }
        } else {
            startOffset = depthOffset;
        }
        value = append(
                moku::slice_expr<moku::slice<uint8_t>>(data, startOffset),
                append(createInsertComponent(
                               moku::slice_expr<moku::slice<std::string>>(
                                       keys, depth),
                               setValue,
                               comma,
                               object),
                       moku::slice_expr<moku::slice<uint8_t>>(data,
                                                              depthOffset)));
    } else {
        startComponent =
                moku::slice_expr<moku::slice<uint8_t>>(data, startOffset);
        endComponent = moku::slice_expr<moku::slice<uint8_t>>(data, endOffset);
        value = make(moku::slice<byte>,
                     len(startComponent) + len(endComponent) + len(setValue));
        newEndOffset = startOffset + len(setValue);
        copy(moku::slice_expr<moku::slice<uint8_t>>(value, 0, startOffset),
             startComponent);
        copy(moku::slice_expr<moku::slice<uint8_t>>(
                     value, startOffset, newEndOffset),
             setValue);
        copy(moku::slice_expr<moku::slice<uint8_t>>(value, newEndOffset),
             endComponent);
    }
    return {value, nil};
}
std::tuple<moku::slice<uint8_t>, ValueType, int, moku::error> getType(
        moku::slice<uint8_t> data, int offset)
{
    ValueType dataType{};
    int endOffset{0};

    endOffset = offset;
    if (data[offset] == '"') {
        dataType = String;
        {
            int idx{0};

            std::tie(idx, _) = stringEnd(
                    moku::slice_expr<moku::slice<uint8_t>>(data, offset + 1));

            if (idx != -1) {
                endOffset += idx + 1;
            } else {
                return {nil, dataType, offset, MalformedStringError};
            }
        }
    } else {
        if (data[offset] == '[') {
            dataType = Array;
            endOffset = blockEnd(
                    moku::slice_expr<moku::slice<uint8_t>>(data, offset),
                    '[',
                    ']');
            if (endOffset == -1) {
                return {nil, dataType, offset, MalformedArrayError};
            }
            endOffset += offset;
        } else {
            if (data[offset] == '{') {
                dataType = Object;
                endOffset = blockEnd(
                        moku::slice_expr<moku::slice<uint8_t>>(data, offset),
                        '{',
                        '}');
                if (endOffset == -1) {
                    return {nil, dataType, offset, MalformedObjectError};
                }
                endOffset += offset;
            } else {
                end = tokenEnd(moku::slice_expr<moku::slice<uint8_t>>(
                        data, endOffset));
                if (end == -1) {
                    return {nil, dataType, offset, MalformedValueError};
                }
                value = moku::slice_expr<moku::slice<uint8_t>>(
                        data, offset, endOffset + end);
                if ((data[offset] == 't') || (data[offset] == 'f')) {
                _ident_32_:
                    if (bytes::Equal(value, trueLiteral) ||
                        bytes::Equal(value, falseLiteral)) {
                        dataType = Boolean;
                    } else {
                        return {nil, Unknown, offset, UnknownValueTypeError};
                    }
                } else if ((data[offset] == 'u') || (data[offset] == 'n')) {
                _ident_33_:
                    if (bytes::Equal(value, nullLiteral)) {
                        dataType = Null;
                    } else {
                        return {nil, Unknown, offset, UnknownValueTypeError};
                    }
                } else if ((data[offset] == '0') || (data[offset] == '1') ||
                           (data[offset] == '2') || (data[offset] == '3') ||
                           (data[offset] == '4') || (data[offset] == '5') ||
                           (data[offset] == '6') || (data[offset] == '7') ||
                           (data[offset] == '8') || (data[offset] == '9') ||
                           (data[offset] == '-')) {
                _ident_34_:
                    dataType = Number;
                } else {
                _ident_35_:
                    return {nil, Unknown, offset, UnknownValueTypeError};
                }
                endOffset += end;
            }
        }
    }
    return {moku::slice_expr<moku::slice<uint8_t>>(data, offset, endOffset),
            dataType,
            endOffset,
            nil};
}
std::tuple<moku::slice<uint8_t>, ValueType, int, moku::error> Get(
        moku::slice<uint8_t> data, moku::slice<std::string> keys)
{
    moku::slice<uint8_t> a{};
    ValueType b{};
    int d{0};
    ValueType dataType{};
    moku::error e{};
    moku::error err{};
    int offset{0};
    moku::slice<uint8_t> value{};

    std::tie(a, b, _, d, e) = internalGet(data, keys);
    return {a, b, d, e};
}
std::tuple<moku::slice<uint8_t>, ValueType, int, int, moku::error> internalGet(
        moku::slice<uint8_t> data, moku::slice<std::string> keys)
{
    ValueType dataType{};
    int endOffset{0};
    moku::error err{};
    int nO{0};
    int offset{0};
    moku::slice<uint8_t> value{};

    if (len(keys) > 0) {
        {
            offset = searchKeys(data, keys);

            if (offset == -1) {
                return {nil, NotExist, -1, -1, KeyPathNotFoundError};
            }
        }
    }
    nO = nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, offset));
    if (nO == -1) {
        return {nil, NotExist, offset, -1, MalformedJsonError};
    }
    offset += nO;
    std::tie(value, dataType, endOffset, err) = getType(data, offset);
    if (!moku::is_nil(err)) {
        return {value, dataType, offset, endOffset, err};
    }
    if (dataType == String) {
        value = moku::slice_expr<moku::slice<uint8_t>>(
                value, 1, len(value) - 1);
    }
    return {value, dataType, offset, endOffset, nil};
}
std::tuple<int, moku::error> ArrayEach(
        moku::slice<uint8_t> data,
        std::function<void(moku::slice<uint8_t>, ValueType, int, moku::error)>
                cb,
        moku::slice<std::string> keys)
{
    moku::error err{};
    int nO{0};
    int offset{0};

    if (len(data) == 0) {
        return {-1, MalformedObjectError};
    }
    offset = 1;
    if (len(keys) > 0) {
        {
            offset = searchKeys(data, keys);

            if (offset == -1) {
                return {offset, KeyPathNotFoundError};
            }
        }
        nO = nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, offset));
        if (nO == -1) {
            return {offset, MalformedJsonError};
        }
        offset += nO;
        if (data[offset] != '[') {
            return {offset, MalformedArrayError};
        }
        offset++;
    }
    nO = nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, offset));
    if (nO == -1) {
        return {offset, MalformedJsonError};
    }
    offset += nO;
    if (data[offset] == ']') {
        return {offset, nil};
    }
    while (true) {
        moku::error e{};
        int o{0};
        int skipToToken{0};
        ValueType t{};
        moku::slice<uint8_t> v{};

        std::tie(v, t, o, e) =
                Get(moku::slice_expr<moku::slice<uint8_t>>(data, offset));
        if (!moku::is_nil(e)) {
            return {offset, e};
        }
        if (o == 0) {
            break;
        }
        if (t != NotExist) {
            cb(v, t, offset + o - len(v), e);
        }
        if (!moku::is_nil(e)) {
            break;
        }
        offset += o;
        skipToToken =
                nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, offset));
        if (skipToToken == -1) {
            return {offset, MalformedArrayError};
        }
        offset += skipToToken;
        if (data[offset] == ']') {
            break;
        }
        if (data[offset] != ',') {
            return {offset, MalformedArrayError};
        }
        offset++;
    }
    return {offset, nil};
}
moku::error ObjectEach(
        moku::slice<uint8_t> data,
        std::function<moku::error(
                moku::slice<uint8_t>, moku::slice<uint8_t>, ValueType, int)>
                callback,
        moku::slice<std::string> keys)
{
    moku::error err{};
    int offset{0};
    std::vector<uint8_t> stackbuf{};

    offset = 0;
    if (len(keys) > 0) {
        {
            int off{0};

            off = searchKeys(data, keys);

            if (off == -1) {
                return KeyPathNotFoundError;
            } else {
                offset = off;
            }
        }
    }
    {
        int off{0};

        off = nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, offset));

        if (off == -1) {
            return MalformedObjectError;
        } else {
            {
                offset += off;

                if (data[offset] != '{') {
                    return MalformedObjectError;
                } else {
                    offset++;
                }
            }
        }
    }
    {
        int off{0};

        off = nextToken(moku::slice_expr<moku::slice<uint8_t>>(data, offset));

        if (off == -1) {
            return MalformedJsonError;
        } else {
            {
                offset += off;

                if (data[offset] == '}') {
                    return nil;
                }
            }
        }
    }
    while (offset < len(data)) {
        moku::slice<uint8_t> key{};
        bool keyEscaped{false};

        if ((data[offset] == '"')) {
        _ident_36_:
            offset++;
        } else if ((data[offset] == '}')) {
        _ident_37_:
            return nil;
        } else {
        _ident_38_:
            return MalformedObjectError;
        }
        {
            bool esc{false};
            int off{0};

            std::tie(off, esc) = stringEnd(
                    moku::slice_expr<moku::slice<uint8_t>>(data, offset));

            if (off == -1) {
                return MalformedJsonError;
            } else {
                std::tie(key, keyEscaped) =
                        std::tuple<moku::slice<uint8_t>, bool>(
                                moku::slice_expr<moku::slice<uint8_t>>(
                                        data, offset, offset + off - 1),
                                esc);
                offset += off;
            }
        }
        if (keyEscaped) {
            {
                moku::error err{};
                moku::slice<uint8_t> keyUnescaped{};

                std::tie(keyUnescaped, err) = Unescape(
                        key, moku::slice_expr<std::vector<uint8_t>>(stackbuf));

                if (!moku::is_nil(err)) {
                    return MalformedStringEscapeError;
                } else {
                    key = keyUnescaped;
                }
            }
        }
        {
            int off{0};

            off = nextToken(
                    moku::slice_expr<moku::slice<uint8_t>>(data, offset));

            if (off == -1) {
                return MalformedJsonError;
            } else {
                {
                    offset += off;

                    if (data[offset] != ':') {
                        return MalformedJsonError;
                    } else {
                        offset++;
                    }
                }
            }
        }
        {
            moku::error err{};
            int off{0};
            moku::slice<uint8_t> value{};
            ValueType valueType{};

            std::tie(value, valueType, off, err) =
                    Get(moku::slice_expr<moku::slice<uint8_t>>(data, offset));

            if (!moku::is_nil(err)) {
                return err;
            } else {
                {
                    moku::error err{};

                    err = callback(key, value, valueType, offset + off);

                    if (!moku::is_nil(err)) {
                        return err;
                    } else {
                        offset += off;
                    }
                }
            }
        }
        {
            int off{0};

            off = nextToken(
                    moku::slice_expr<moku::slice<uint8_t>>(data, offset));

            if (off == -1) {
                return MalformedArrayError;
            } else {
                offset += off;
                if ((data[offset] == '}')) {
                _ident_39_:
                    return nil;
                } else if ((data[offset] == ',')) {
                _ident_40_:
                    offset++;
                } else {
                _ident_41_:
                    return MalformedObjectError;
                }
            }
        }
        {
            int off{0};

            off = nextToken(
                    moku::slice_expr<moku::slice<uint8_t>>(data, offset));

            if (off == -1) {
                return MalformedArrayError;
            } else {
                offset += off;
            }
        }
    }
    return MalformedObjectError;
}
std::tuple<std::string, moku::error> GetUnsafeString(
        moku::slice<uint8_t> data, moku::slice<std::string> keys)
{
    moku::error e{};
    moku::error err{};
    moku::slice<uint8_t> v{};
    std::string val{""};

    std::tie(v, _, _, e) = Get(data, keys);
    if (!moku::is_nil(e)) {
        return {"", e};
    }
    return {bytesToString(&v), nil};
}
std::tuple<std::string, moku::error> GetString(moku::slice<uint8_t> data,
                                               moku::slice<std::string> keys)
{
    moku::error e{};
    moku::error err{};
    ValueType t{};
    moku::slice<uint8_t> v{};
    std::string val{""};

    std::tie(v, t, _, e) = Get(data, keys);
    if (!moku::is_nil(e)) {
        return {"", e};
    }
    if (t != String) {
        return {"", fmt::Errorf("Value is not a string: %s", std::string(v))};
    }
    if (bytes::IndexByte(v, '\\') == -1) {
        return {std::string(v), nil};
    }
    return ParseString(v);
}
std::tuple<double, moku::error> GetFloat(moku::slice<uint8_t> data,
                                         moku::slice<std::string> keys)
{
    moku::error e{};
    moku::error err{};
    ValueType t{};
    moku::slice<uint8_t> v{};
    double val{0};

    std::tie(v, t, _, e) = Get(data, keys);
    if (!moku::is_nil(e)) {
        return {0, e};
    }
    if (t != Number) {
        return {0, fmt::Errorf("Value is not a number: %s", std::string(v))};
    }
    return ParseFloat(v);
}
std::tuple<int64_t, moku::error> GetInt(moku::slice<uint8_t> data,
                                        moku::slice<std::string> keys)
{
    moku::error e{};
    moku::error err{};
    ValueType t{};
    moku::slice<uint8_t> v{};
    int64_t val{0};

    std::tie(v, t, _, e) = Get(data, keys);
    if (!moku::is_nil(e)) {
        return {0, e};
    }
    if (t != Number) {
        return {0, fmt::Errorf("Value is not a number: %s", std::string(v))};
    }
    return ParseInt(v);
}
std::tuple<bool, moku::error> GetBoolean(moku::slice<uint8_t> data,
                                         moku::slice<std::string> keys)
{
    moku::error e{};
    moku::error err{};
    ValueType t{};
    moku::slice<uint8_t> v{};
    bool val{false};

    std::tie(v, t, _, e) = Get(data, keys);
    if (!moku::is_nil(e)) {
        return {false, e};
    }
    if (t != Boolean) {
        return {false,
                fmt::Errorf("Value is not a boolean: %s", std::string(v))};
    }
    return ParseBoolean(v);
}
std::tuple<bool, moku::error> ParseBoolean(moku::slice<uint8_t> b)
{
    if ((bytes::Equal(b, trueLiteral))) {
    _ident_42_:
        return {true, nil};
    } else if ((bytes::Equal(b, falseLiteral))) {
    _ident_43_:
        return {false, nil};
    } else {
    _ident_44_:
        return {false, MalformedValueError};
    }
}
std::tuple<std::string, moku::error> ParseString(moku::slice<uint8_t> b)
{
    std::vector<uint8_t> stackbuf{};

    {
        moku::slice<uint8_t> bU{};
        moku::error err{};

        std::tie(bU, err) =
                Unescape(b, moku::slice_expr<std::vector<uint8_t>>(stackbuf));

        if (!moku::is_nil(err)) {
            return {"", MalformedValueError};
        } else {
            return {std::string(bU), nil};
        }
    }
}
std::tuple<double, moku::error> ParseFloat(moku::slice<uint8_t> b)
{
    {
        moku::error err{};
        double v{0};

        std::tie(v, err) = parseFloat(&b);

        if (!moku::is_nil(err)) {
            return {0, MalformedValueError};
        } else {
            return {v, nil};
        }
    }
}
std::tuple<int64_t, moku::error> ParseInt(moku::slice<uint8_t> b)
{
    {
        bool ok{false};
        int64_t v{0};

        std::tie(v, ok) = parseInt(b);

        if (!ok) {
            return {0, MalformedValueError};
        } else {
            return {v, nil};
        }
    }
}
void init();
int main()
{
    init();
    backslashCharEscapeTable = std::vector<byte>{{'"', '"'},
                                                 {'\\', '\\'},
                                                 {'/', '/'},
                                                 {'b', '\b'},
                                                 {'f', '\f'},
                                                 {'n', '\n'},
                                                 {'r', '\r'},
                                                 {'t', '\t'}};
    KeyPathNotFoundError = errors::New("Key path not found");
    UnknownValueTypeError = errors::New("Unknown value type");
    MalformedJsonError = errors::New("Malformed JSON error");
    MalformedStringError =
            errors::New("Value is string, but can't find closing '\"' symbol");
    MalformedArrayError =
            errors::New("Value is array, but can't find closing ']' symbol");
    MalformedObjectError = errors::New(
            "Value looks like object, but can't find closing '}' symbol");
    MalformedValueError = errors::New(
            "Value looks like Number/Boolean/None, but can't find its end: ',' "
            "or '}' symbol");
    MalformedStringEscapeError =
            errors::New("Encountered an invalid escape sequence in a string");
    trueLiteral = moku::slice<byte>("true");
    falseLiteral = moku::slice<byte>("false");
    nullLiteral = moku::slice<byte>("null");
    _main();
    return 0;
}
