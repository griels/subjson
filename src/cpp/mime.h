#pragma once
#include "base64.h"
#include "bufio.h"
#include "bytes.h"
#include "errors.h"
#include "fmt.h"
#include "io.h"
#include "os.h"
#include "sort.h"
#include "strings.h"
#include "sync.h"
#include "unicode.h"
#include "utf8.h"
namespace mime {
moku::error AddExtensionType(std::string ext, std::string typ);
constexpr WordEncoder BEncoding{98};
moku::error ErrInvalidMediaParameter;
std::tuple<moku::slice<std::string>, moku::error> ExtensionsByType(
        std::string typ);
std::string FormatMediaType(std::string t,
                            std::map<std::string, std::string> param);
std::tuple<std::string, std::map<std::string, std::string>, moku::error>
ParseMediaType(std::string v);
constexpr WordEncoder QEncoding{113};
std::string TypeByExtension(std::string ext);

struct WordDecoder {
    std::function<Reader, moku::error(std::string, Reader)> CharsetReader{
            std::nullptr};
    std::tuple<std::string, moku::error> Decode(std::string word);
    std::tuple<std::string, moku::error> DecodeHeader(std::string header);
    moku::error convert(Buffer *buf,
                        std::string charset,
                        moku::slice<uint8_t> content);
};
template <>
inline bool moku::is_nil<WordDecoder>(const WordDecoder &worddecoder)
{
    return moku::is_nil<
            std::function<Reader, moku::error(std::string, Reader)>>(
            worddecoder.CharsetReader);
}
struct WordEncoder : public moku::basic<uint8_t> {
    WordEncoder() : moku::basic<uint8_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<WordEncoder>(const WordEncoder &wordencoder)
{
    return moku::is_nil<uint8_t>(uint8_t(wordencoder));
}
inline std::string Encode(std::string charset, std::string s)
{
    WordEncoder wordencoder = *this;
    return wordencoder._EncodeByValue(std::string charset, std::string s);
}
std::string _EncodeByValue(std::string charset, std::string s);
inline std::string encodeWord(std::string charset, std::string s)
{
    WordEncoder wordencoder = *this;
    return wordencoder._encodeWordByValue(std::string charset, std::string s);
}
std::string _encodeWordByValue(std::string charset, std::string s);
inline void bEncode(Buffer *buf, std::string charset, std::string s)
{
    WordEncoder wordencoder = *this;
    wordencoder._bEncodeByValue(
            Buffer * buf, std::string charset, std::string s);
}
void _bEncodeByValue(Buffer *buf, std::string charset, std::string s);
inline void qEncode(Buffer *buf, std::string charset, std::string s)
{
    WordEncoder wordencoder = *this;
    wordencoder._qEncodeByValue(
            Buffer * buf, std::string charset, std::string s);
}
void _qEncodeByValue(Buffer *buf, std::string charset, std::string s);
inline void openWord(Buffer *buf, std::string charset)
{
    WordEncoder wordencoder = *this;
    wordencoder._openWordByValue(Buffer * buf, std::string charset);
}
void _openWordByValue(Buffer *buf, std::string charset);
inline void splitWord(Buffer *buf, std::string charset)
{
    WordEncoder wordencoder = *this;
    wordencoder._splitWordByValue(Buffer * buf, std::string charset);
}
void _splitWordByValue(Buffer *buf, std::string charset);
static Pool bufPool;
static std::map<std::string, std::string> builtinTypesLower;
moku::error checkMediaTypeDisposition(std::string s);
void clearSyncMap(Map *m);
void closeWord(Buffer *buf);
std::tuple<std::string, std::string, std::string> consumeMediaParam(
        std::string v);
std::tuple<std::string, std::string> consumeToken(std::string v);
std::tuple<std::string, std::string> consumeValue(std::string v);
std::tuple<moku::slice<uint8_t>, moku::error> decode(uint8_t encoding,
                                                     std::string text);
std::string decode2231Enc(std::string v);
static moku::error errInvalidWord;
static Map extensions;
static Mutex extensionsMu;
std::tuple<uint8_t, moku::error> fromHex(uint8_t b);
Buffer *getBuffer();
bool hasNonWhitespace(std::string s);
void initMime();
std::map<std::string, std::string> initMimeForTests();
void initMimeUnix();
bool isNotTokenChar(int32_t r);
bool isTSpecial(int32_t r);
bool isToken(std::string s);
bool isTokenChar(int32_t r);
bool isUTF8(std::string charset);
bool ishex(uint8_t c);
void loadMimeFile(std::string filename);
static int maxBase64Len;
static constexpr int maxContentLen{63};
static constexpr int maxEncodedWordLen{75};
static Map mimeTypes;
static Map mimeTypesLower;
bool needsEncoding(std::string s);
static Once once;
static std::function<void()> osInitMime;
std::tuple<std::string, moku::error> percentHexUnescape(std::string s);
void putBuffer(Buffer *buf);
std::tuple<moku::slice<uint8_t>, moku::error> qDecode(std::string s);
std::tuple<uint8_t, moku::error> readHexByte(uint8_t a, uint8_t b);
moku::error setExtensionType(std::string extension, std::string mimeType);
void setMimeTypes(std::map<std::string, std::string> lowerExt,
                  std::map<std::string, std::string> mixExt);
static std::function<void()> testInitMime;
static moku::slice<std::string> typeFiles;
uint8_t unhex(uint8_t c);
static constexpr std::string upperhex{"0123456789ABCDEF"};
void writeQString(Buffer *buf, std::string s);
} // namespace mime
