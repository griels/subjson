#pragma once
#include "bytes.h"
#include "errors.h"
#include "fmt.h"
#include "math.h"
#include "reflect.h"
#include "strconv.h"
#include "unsafe.h"
#include "utf8.h"
namespace jsonparser {
constexpr ValueType Array{4};
std::tuple<int, moku::error> ArrayEach(
        moku::slice<uint8_t> data,
        std::function<void(moku::slice<uint8_t>, ValueType, int, moku::error)>
                cb,
        moku::slice<std::string> keys);
constexpr ValueType Boolean{5};
moku::slice<uint8_t> Delete(moku::slice<uint8_t> data,
                            moku::slice<std::string> keys);
int EachKey(
        moku::slice<uint8_t> data,
        std::function<void(int, moku::slice<uint8_t>, ValueType, moku::error)>
                cb,
        moku::slice<moku::slice<std::string>> paths);
std::tuple<moku::slice<uint8_t>, ValueType, int, moku::error> Get(
        moku::slice<uint8_t> data, moku::slice<std::string> keys);
std::tuple<bool, moku::error> GetBoolean(moku::slice<uint8_t> data,
                                         moku::slice<std::string> keys);
std::tuple<double, moku::error> GetFloat(moku::slice<uint8_t> data,
                                         moku::slice<std::string> keys);
std::tuple<int64_t, moku::error> GetInt(moku::slice<uint8_t> data,
                                        moku::slice<std::string> keys);
std::tuple<std::string, moku::error> GetString(moku::slice<uint8_t> data,
                                               moku::slice<std::string> keys);
std::tuple<std::string, moku::error> GetUnsafeString(
        moku::slice<uint8_t> data, moku::slice<std::string> keys);
moku::error KeyPathNotFoundError;
moku::error MalformedArrayError;
moku::error MalformedJsonError;
moku::error MalformedObjectError;
moku::error MalformedStringError;
moku::error MalformedStringEscapeError;
moku::error MalformedValueError;
constexpr ValueType NotExist{0};
constexpr ValueType Null{6};
constexpr ValueType Number{2};
constexpr ValueType Object{3};
moku::error ObjectEach(
        moku::slice<uint8_t> data,
        std::function<moku::error(
                moku::slice<uint8_t>, moku::slice<uint8_t>, ValueType, int)>
                callback,
        moku::slice<std::string> keys);
std::tuple<bool, moku::error> ParseBoolean(moku::slice<uint8_t> b);
std::tuple<double, moku::error> ParseFloat(moku::slice<uint8_t> b);
std::tuple<int64_t, moku::error> ParseInt(moku::slice<uint8_t> b);
std::tuple<std::string, moku::error> ParseString(moku::slice<uint8_t> b);
std::tuple<moku::slice<uint8_t>, moku::error> Set(
        moku::slice<uint8_t> data,
        moku::slice<uint8_t> setValue,
        moku::slice<std::string> keys);
constexpr ValueType String{1};
moku::slice<uint8_t> StringToBytes(std::string s);
std::tuple<moku::slice<uint8_t>, moku::error> Unescape(
        moku::slice<uint8_t> in, moku::slice<uint8_t> out);
constexpr ValueType Unknown{7};
moku::error UnknownValueTypeError;

struct ValueType : public moku::basic<int> {
    ValueType() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<ValueType>(const ValueType &valuetype)
{
    return moku::is_nil<int>(int(valuetype));
}
inline std::string String()
{
    ValueType valuetype = *this;
    return valuetype._StringByValue();
}
std::string _StringByValue();
static std::vector<uint8_t> backslashCharEscapeTable;
static constexpr int badHex{-1};
static constexpr int basicMultilingualPlaneOffset{65535};
static constexpr int basicMultilingualPlaneReservedOffset{57343};
static moku::slice<int64_t> bitwiseFlags;
int blockEnd(moku::slice<uint8_t> data, uint8_t openSym, uint8_t closeSym);
std::string bytesToString(moku::slice<uint8_t> *b);
int32_t combineUTF16Surrogates(int32_t high, int32_t low);
moku::slice<uint8_t> createInsertComponent(moku::slice<std::string> keys,
                                           moku::slice<uint8_t> setValue,
                                           bool comma,
                                           bool object);
std::tuple<int32_t, bool> decodeSingleUnicodeEscape(moku::slice<uint8_t> in);
std::tuple<int32_t, int> decodeUnicodeEscape(moku::slice<uint8_t> in);
bool equalStr(moku::slice<uint8_t> *b, std::string s);
static moku::slice<uint8_t> falseLiteral;
std::tuple<int, moku::error> findKeyStart(moku::slice<uint8_t> data,
                                          std::string key);
int findTokenStart(moku::slice<uint8_t> data, uint8_t token);
std::tuple<moku::slice<uint8_t>, ValueType, int, moku::error> getType(
        moku::slice<uint8_t> data, int offset);
int h2I(uint8_t c);
static constexpr int highSurrogateOffset{55296};
std::tuple<moku::slice<uint8_t>, ValueType, int, int, moku::error> internalGet(
        moku::slice<uint8_t> data, moku::slice<std::string> keys);
bool isUTF16EncodedRune(int32_t r);
int lastToken(moku::slice<uint8_t> data);
static constexpr int lowSurrogateOffset{56320};
int nextToken(moku::slice<uint8_t> data);
static moku::slice<uint8_t> nullLiteral;
std::tuple<double, moku::error> parseFloat(moku::slice<uint8_t> *b);
std::tuple<int64_t, bool> parseInt(moku::slice<uint8_t> bytes);
bool sameTree(moku::slice<std::string> p1, moku::slice<std::string> p2);
int searchKeys(moku::slice<uint8_t> data, moku::slice<std::string> keys);
std::tuple<int, bool> stringEnd(moku::slice<uint8_t> data);
static constexpr int supplementalPlanesOffset{65536};
int tokenEnd(moku::slice<uint8_t> data);
int tokenStart(moku::slice<uint8_t> data);
static moku::slice<uint8_t> trueLiteral;
static constexpr int unescapeStackBufSize{64};
std::tuple<int, int> unescapeToUTF8(moku::slice<uint8_t> in,
                                    moku::slice<uint8_t> out);
} // namespace jsonparser
