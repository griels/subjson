#pragma once
#include "bytes.h"
#include "errors.h"
#include "fmt.h"
#include "io.h"
#include "sync.h"
namespace hpack {
moku::slice<uint8_t> AppendHuffmanString(moku::slice<uint8_t> dst,
                                         std::string s);

struct Decoder {
    dynamicTable dynTab;
    std::function<void(HeaderField)> emit{std::nullptr};
    bool emitEnabled{false};
    int maxStrLen{0};
    moku::slice<uint8_t> buf;
    Buffer saveBuf;
    void SetMaxStringLength(int n);
    void SetEmitFunc(std::function<void(HeaderField)> emitFunc);
    void SetEmitEnabled(bool v);
    bool EmitEnabled();
    void SetMaxDynamicTableSize(uint32_t v);
    void SetAllowedMaxDynamicTableSize(uint32_t v);
    int maxTableIndex();
    std::tuple<HeaderField, bool> at(uint64_t i);
    std::tuple<moku::slice<HeaderField>, moku::error> DecodeFull(
            moku::slice<uint8_t> p);
    moku::error Close();
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> p);
    moku::error parseHeaderFieldRepr();
    moku::error parseFieldIndexed();
    moku::error parseFieldLiteral(uint8_t n, indexType it);
    moku::error callEmit(HeaderField hf);
    moku::error parseDynamicTableSizeUpdate();
    std::tuple<std::string, moku::slice<uint8_t>, moku::error> readString(
            moku::slice<uint8_t> p, bool wantStr);
};
template <>
inline bool moku::is_nil<Decoder>(const Decoder &decoder)
{
    return moku::is_nil<dynamicTable>(decoder.dynTab) &&
           moku::is_nil<std::function<void(HeaderField)>>(decoder.emit) &&
           moku::is_nil<bool>(decoder.emitEnabled) &&
           moku::is_nil<int>(decoder.maxStrLen) &&
           moku::is_nil<moku::slice<uint8_t>>(decoder.buf) &&
           moku::is_nil<Buffer>(decoder.saveBuf);
}
struct DecodingError {
    moku::error Err;
    inline std::string Error()
    {
        DecodingError decodingerror = *this;
        return decodingerror._ErrorByValue();
    }
    std::string _ErrorByValue();
};
template <>
inline bool moku::is_nil<DecodingError>(const DecodingError &decodingerror)
{
    return moku::is_nil<moku::error>(decodingerror.Err);
}
struct Encoder {
    dynamicTable dynTab;
    uint32_t minSize{0};
    uint32_t maxSizeLimit{0};
    bool tableSizeUpdate{false};
    Writer w;
    moku::slice<uint8_t> buf;
    moku::error WriteField(HeaderField f);
    std::tuple<uint64_t, bool> searchTable(HeaderField f);
    void SetMaxDynamicTableSize(uint32_t v);
    void SetMaxDynamicTableSizeLimit(uint32_t v);
    bool shouldIndex(HeaderField f);
};
template <>
inline bool moku::is_nil<Encoder>(const Encoder &encoder)
{
    return moku::is_nil<dynamicTable>(encoder.dynTab) &&
           moku::is_nil<uint32_t>(encoder.minSize) &&
           moku::is_nil<uint32_t>(encoder.maxSizeLimit) &&
           moku::is_nil<bool>(encoder.tableSizeUpdate) &&
           moku::is_nil<Writer>(encoder.w) &&
           moku::is_nil<moku::slice<uint8_t>>(encoder.buf);
}
moku::error ErrInvalidHuffman;
moku::error ErrStringLength;

struct HeaderField {
    std::string Name{""};
    std::string Value{""};
    bool Sensitive{false};
    inline bool IsPseudo()
    {
        HeaderField headerfield = *this;
        return headerfield._IsPseudoByValue();
    }
    bool _IsPseudoByValue();
    inline std::string String()
    {
        HeaderField headerfield = *this;
        return headerfield._StringByValue();
    }
    std::string _StringByValue();
    inline uint32_t Size()
    {
        HeaderField headerfield = *this;
        return headerfield._SizeByValue();
    }
    uint32_t _SizeByValue();
};
template <>
inline bool moku::is_nil<HeaderField>(const HeaderField &headerfield)
{
    return moku::is_nil<std::string>(headerfield.Name) &&
           moku::is_nil<std::string>(headerfield.Value) &&
           moku::is_nil<bool>(headerfield.Sensitive);
}
std::tuple<int, moku::error> HuffmanDecode(Writer w, moku::slice<uint8_t> v);
std::tuple<std::string, moku::error> HuffmanDecodeToString(
        moku::slice<uint8_t> v);
uint64_t HuffmanEncodeLength(std::string s);

struct InvalidIndexError : public moku::basic<int> {
    InvalidIndexError() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<InvalidIndexError>(
        const InvalidIndexError &invalidindexerror)
{
    return moku::is_nil<int>(int(invalidindexerror));
}
inline std::string Error()
{
    InvalidIndexError invalidindexerror = *this;
    return invalidindexerror._ErrorByValue();
}
std::string _ErrorByValue();
Decoder *NewDecoder(uint32_t maxDynamicTableSize,
                    std::function<void(HeaderField)> emitFunc);
Encoder *NewEncoder(Writer w);
void addDecoderNode(uint8_t sym, uint32_t code, uint8_t codeLen);
std::tuple<moku::slice<uint8_t>, uint8_t> appendByteToHuffmanCode(
        moku::slice<uint8_t> dst, uint8_t rembits, uint8_t c);
moku::slice<uint8_t> appendHpackString(moku::slice<uint8_t> dst, std::string s);
moku::slice<uint8_t> appendIndexed(moku::slice<uint8_t> dst, uint64_t i);
moku::slice<uint8_t> appendIndexedName(moku::slice<uint8_t> dst,
                                       HeaderField f,
                                       uint64_t i,
                                       bool indexing);
moku::slice<uint8_t> appendNewName(moku::slice<uint8_t> dst,
                                   HeaderField f,
                                   bool indexing);
moku::slice<uint8_t> appendTableSize(moku::slice<uint8_t> dst, uint32_t v);
moku::slice<uint8_t> appendVarInt(moku::slice<uint8_t> dst,
                                  uint8_t n,
                                  uint64_t i);
static Pool bufPool;

struct dynamicTable {
    headerFieldTable table;
    uint32_t size{0};
    uint32_t maxSize{0};
    uint32_t allowedMaxSize{0};
    void setMaxSize(uint32_t v);
    void add(HeaderField f);
    void evict();
};
template <>
inline bool moku::is_nil<dynamicTable>(const dynamicTable &dynamictable)
{
    return moku::is_nil<headerFieldTable>(dynamictable.table) &&
           moku::is_nil<uint32_t>(dynamictable.size) &&
           moku::is_nil<uint32_t>(dynamictable.maxSize) &&
           moku::is_nil<uint32_t>(dynamictable.allowedMaxSize);
}
uint8_t encodeTypeByte(bool indexing, bool sensitive);
static moku::error errNeedMore;
static DecodingError errVarintOverflow;

struct headerFieldTable {
    moku::slice<HeaderField> ents;
    uint64_t evictCount{0};
    std::map<std::string, uint64_t> byName;
    std::map<pairNameValue, uint64_t> byNameValue;
    void init();
    int len();
    void addEntry(HeaderField f);
    void evictOldest(int n);
    std::tuple<uint64_t, bool> search(HeaderField f);
    uint64_t idToIndex(uint64_t id);
};
template <>
inline bool moku::is_nil<headerFieldTable>(
        const headerFieldTable &headerfieldtable)
{
    return moku::is_nil<moku::slice<HeaderField>>(headerfieldtable.ents) &&
           moku::is_nil<uint64_t>(headerfieldtable.evictCount) &&
           moku::is_nil<std::map<std::string, uint64_t>>(
                   headerfieldtable.byName) &&
           moku::is_nil<std::map<pairNameValue, uint64_t>>(
                   headerfieldtable.byNameValue);
}
static std::vector<uint8_t> huffmanCodeLen;
static std::vector<uint32_t> huffmanCodes;
moku::error huffmanDecode(Buffer *buf, int maxLen, moku::slice<uint8_t> v);

struct indexType : public moku::basic<int> {
    indexType() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<indexType>(const indexType &indextype)
{
    return moku::is_nil<int>(int(indextype));
}
inline bool indexed()
{
    indexType indextype = *this;
    return indextype._indexedByValue();
}
bool _indexedByValue();
inline bool sensitive()
{
    indexType indextype = *this;
    return indextype._sensitiveByValue();
}
bool _sensitiveByValue();
static constexpr indexType indexedFalse{1};
static constexpr indexType indexedNever{2};
static constexpr indexType indexedTrue{0};
static constexpr int initialHeaderTableSize{4096};
node *newInternalNode();
headerFieldTable *newStaticTable();

struct node {
    moku::slice<node *> children;
    uint8_t codeLen{0};
    uint8_t sym{0};
};
template <>
inline bool moku::is_nil<node>(const node &node)
{
    return moku::is_nil<moku::slice<node *>>(node.children) &&
           moku::is_nil<uint8_t>(node.codeLen) &&
           moku::is_nil<uint8_t>(node.sym);
}
struct pairNameValue {
    std::string name{""};
    std::string value{""};
};
template <>
inline bool moku::is_nil<pairNameValue>(const pairNameValue &pairnamevalue)
{
    return moku::is_nil<std::string>(pairnamevalue.name) &&
           moku::is_nil<std::string>(pairnamevalue.value);
}
std::tuple<uint64_t, moku::slice<uint8_t>, moku::error> readVarInt(
        uint8_t n, moku::slice<uint8_t> p);
static node *rootHuffmanNode;
static headerFieldTable *staticTable;
static std::vector<HeaderField> staticTableEntries;
static constexpr uint32_t uint32Max{4294967295};
} // namespace hpack
