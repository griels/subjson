#pragma once
#include "fmt.h"
#include "io.h"
#include "transform.h"
#include "utf8.h"
namespace norm {

struct Form : public moku::basic<int> {
    Form() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<Form>(const Form &form)
{
    return moku::is_nil<int>(int(form));
}
inline Properties Properties(moku::slice<uint8_t> s)
{
    Form form = *this;
    return form._PropertiesByValue(moku::slice<uint8_t> s);
}
Properties _PropertiesByValue(moku::slice<uint8_t> s);
inline Properties PropertiesString(std::string s)
{
    Form form = *this;
    return form._PropertiesStringByValue(std::string s);
}
Properties _PropertiesStringByValue(std::string s);
inline moku::slice<uint8_t> Bytes(moku::slice<uint8_t> b)
{
    Form form = *this;
    return form._BytesByValue(moku::slice<uint8_t> b);
}
moku::slice<uint8_t> _BytesByValue(moku::slice<uint8_t> b);
inline std::string String(std::string s)
{
    Form form = *this;
    return form._StringByValue(std::string s);
}
std::string _StringByValue(std::string s);
inline bool IsNormal(moku::slice<uint8_t> b)
{
    Form form = *this;
    return form._IsNormalByValue(moku::slice<uint8_t> b);
}
bool _IsNormalByValue(moku::slice<uint8_t> b);
inline bool IsNormalString(std::string s)
{
    Form form = *this;
    return form._IsNormalStringByValue(std::string s);
}
bool _IsNormalStringByValue(std::string s);
inline moku::slice<uint8_t> Append(moku::slice<uint8_t> out,
                                   moku::slice<uint8_t> src)
{
    Form form = *this;
    return form._AppendByValue(moku::slice<uint8_t> out,
                               moku::slice<uint8_t> src);
}
moku::slice<uint8_t> _AppendByValue(moku::slice<uint8_t> out,
                                    moku::slice<uint8_t> src);
inline moku::slice<uint8_t> doAppend(moku::slice<uint8_t> out, input src, int n)
{
    Form form = *this;
    return form._doAppendByValue(moku::slice<uint8_t> out, input src, int n);
}
moku::slice<uint8_t> _doAppendByValue(moku::slice<uint8_t> out,
                                      input src,
                                      int n);
inline moku::slice<uint8_t> AppendString(moku::slice<uint8_t> out,
                                         std::string src)
{
    Form form = *this;
    return form._AppendStringByValue(moku::slice<uint8_t> out, std::string src);
}
moku::slice<uint8_t> _AppendStringByValue(moku::slice<uint8_t> out,
                                          std::string src);
inline int QuickSpan(moku::slice<uint8_t> b)
{
    Form form = *this;
    return form._QuickSpanByValue(moku::slice<uint8_t> b);
}
int _QuickSpanByValue(moku::slice<uint8_t> b);
inline std::tuple<int, moku::error> Span(moku::slice<uint8_t> b, bool atEOF)
{
    Form form = *this;
    return form._SpanByValue(moku::slice<uint8_t> b, bool atEOF);
}
std::tuple<int, moku::error> _SpanByValue(moku::slice<uint8_t> b, bool atEOF);
inline std::tuple<int, moku::error> SpanString(std::string s, bool atEOF)
{
    Form form = *this;
    return form._SpanStringByValue(std::string s, bool atEOF);
}
std::tuple<int, moku::error> _SpanStringByValue(std::string s, bool atEOF);
inline int QuickSpanString(std::string s)
{
    Form form = *this;
    return form._QuickSpanStringByValue(std::string s);
}
int _QuickSpanStringByValue(std::string s);
inline int FirstBoundary(moku::slice<uint8_t> b)
{
    Form form = *this;
    return form._FirstBoundaryByValue(moku::slice<uint8_t> b);
}
int _FirstBoundaryByValue(moku::slice<uint8_t> b);
inline int firstBoundary(input src, int nsrc)
{
    Form form = *this;
    return form._firstBoundaryByValue(input src, int nsrc);
}
int _firstBoundaryByValue(input src, int nsrc);
inline int FirstBoundaryInString(std::string s)
{
    Form form = *this;
    return form._FirstBoundaryInStringByValue(std::string s);
}
int _FirstBoundaryInStringByValue(std::string s);
inline int NextBoundary(moku::slice<uint8_t> b, bool atEOF)
{
    Form form = *this;
    return form._NextBoundaryByValue(moku::slice<uint8_t> b, bool atEOF);
}
int _NextBoundaryByValue(moku::slice<uint8_t> b, bool atEOF);
inline int NextBoundaryInString(std::string s, bool atEOF)
{
    Form form = *this;
    return form._NextBoundaryInStringByValue(std::string s, bool atEOF);
}
int _NextBoundaryInStringByValue(std::string s, bool atEOF);
inline int nextBoundary(input src, int nsrc, bool atEOF)
{
    Form form = *this;
    return form._nextBoundaryByValue(input src, int nsrc, bool atEOF);
}
int _nextBoundaryByValue(input src, int nsrc, bool atEOF);
inline int LastBoundary(moku::slice<uint8_t> b)
{
    Form form = *this;
    return form._LastBoundaryByValue(moku::slice<uint8_t> b);
}
int _LastBoundaryByValue(moku::slice<uint8_t> b);
inline WriteCloser Writer(Writer w)
{
    Form form = *this;
    return form._WriterByValue(Writer w);
}
WriteCloser _WriterByValue(Writer w);
inline Reader Reader(Reader r)
{
    Form form = *this;
    return form._ReaderByValue(Reader r);
}
Reader _ReaderByValue(Reader r);
inline void Reset()
{
    Form form = *this;
    form._ResetByValue();
}
void _ResetByValue();
inline std::tuple<int, int, moku::error> Transform(moku::slice<uint8_t> dst,
                                                   moku::slice<uint8_t> src,
                                                   bool atEOF)
{
    Form form = *this;
    return form._TransformByValue(
            moku::slice<uint8_t> dst, moku::slice<uint8_t> src, bool atEOF);
}
std::tuple<int, int, moku::error> _TransformByValue(moku::slice<uint8_t> dst,
                                                    moku::slice<uint8_t> src,
                                                    bool atEOF);
inline std::tuple<int, int, moku::error> transform(moku::slice<uint8_t> dst,
                                                   moku::slice<uint8_t> src,
                                                   bool atEOF)
{
    Form form = *this;
    return form._transformByValue(
            moku::slice<uint8_t> dst, moku::slice<uint8_t> src, bool atEOF);
}
std::tuple<int, int, moku::error> _transformByValue(moku::slice<uint8_t> dst,
                                                    moku::slice<uint8_t> src,
                                                    bool atEOF);
constexpr std::string GraphemeJoiner{"͏"};

struct Iter {
    reorderBuffer rb;
    std::vector<uint8_t> buf;
    Properties info;
    iterFunc next;
    iterFunc asciiF;
    int p{0};
    moku::slice<uint8_t> multiSeg;
    void Init(Form f, moku::slice<uint8_t> src);
    void InitString(Form f, std::string src);
    std::tuple<int64_t, moku::error> Seek(int64_t offset, int whence);
    moku::slice<uint8_t> returnSlice(int a, int b);
    int Pos();
    void setDone();
    bool Done();
    moku::slice<uint8_t> Next();
};
template <>
inline bool moku::is_nil<Iter>(const Iter &iter)
{
    return moku::is_nil<reorderBuffer>(iter.rb) &&
           moku::is_nil<std::vector<uint8_t>>(iter.buf) &&
           moku::is_nil<Properties>(iter.info) &&
           moku::is_nil<iterFunc>(iter.next) &&
           moku::is_nil<iterFunc>(iter.asciiF) && moku::is_nil<int>(iter.p) &&
           moku::is_nil<moku::slice<uint8_t>>(iter.multiSeg);
}
constexpr int MaxSegmentSize{128};
constexpr int MaxTransformChunkSize{155};
constexpr Form NFC{0};
constexpr Form NFD{1};
constexpr Form NFKC{2};
constexpr Form NFKD{3};

struct Properties {
    uint8_t pos{0};
    uint8_t size{0};
    uint8_t ccc{0};
    uint8_t tccc{0};
    uint8_t nLead{0};
    qcInfo flags;
    uint16_t index{0};
    inline bool BoundaryBefore()
    {
        Properties properties = *this;
        return properties._BoundaryBeforeByValue();
    }
    bool _BoundaryBeforeByValue();
    inline bool BoundaryAfter()
    {
        Properties properties = *this;
        return properties._BoundaryAfterByValue();
    }
    bool _BoundaryAfterByValue();
    inline bool isYesC()
    {
        Properties properties = *this;
        return properties._isYesCByValue();
    }
    bool _isYesCByValue();
    inline bool isYesD()
    {
        Properties properties = *this;
        return properties._isYesDByValue();
    }
    bool _isYesDByValue();
    inline bool combinesForward()
    {
        Properties properties = *this;
        return properties._combinesForwardByValue();
    }
    bool _combinesForwardByValue();
    inline bool combinesBackward()
    {
        Properties properties = *this;
        return properties._combinesBackwardByValue();
    }
    bool _combinesBackwardByValue();
    inline bool hasDecomposition()
    {
        Properties properties = *this;
        return properties._hasDecompositionByValue();
    }
    bool _hasDecompositionByValue();
    inline bool isInert()
    {
        Properties properties = *this;
        return properties._isInertByValue();
    }
    bool _isInertByValue();
    inline bool multiSegment()
    {
        Properties properties = *this;
        return properties._multiSegmentByValue();
    }
    bool _multiSegmentByValue();
    inline uint8_t nLeadingNonStarters()
    {
        Properties properties = *this;
        return properties._nLeadingNonStartersByValue();
    }
    uint8_t _nLeadingNonStartersByValue();
    inline uint8_t nTrailingNonStarters()
    {
        Properties properties = *this;
        return properties._nTrailingNonStartersByValue();
    }
    uint8_t _nTrailingNonStartersByValue();
    inline moku::slice<uint8_t> Decomposition()
    {
        Properties properties = *this;
        return properties._DecompositionByValue();
    }
    moku::slice<uint8_t> _DecompositionByValue();
    inline int Size()
    {
        Properties properties = *this;
        return properties._SizeByValue();
    }
    int _SizeByValue();
    inline uint8_t CCC()
    {
        Properties properties = *this;
        return properties._CCCByValue();
    }
    uint8_t _CCCByValue();
    inline uint8_t LeadCCC()
    {
        Properties properties = *this;
        return properties._LeadCCCByValue();
    }
    uint8_t _LeadCCCByValue();
    inline uint8_t TrailCCC()
    {
        Properties properties = *this;
        return properties._TrailCCCByValue();
    }
    uint8_t _TrailCCCByValue();
};
template <>
inline bool moku::is_nil<Properties>(const Properties &properties)
{
    return moku::is_nil<uint8_t>(properties.pos) &&
           moku::is_nil<uint8_t>(properties.size) &&
           moku::is_nil<uint8_t>(properties.ccc) &&
           moku::is_nil<uint8_t>(properties.tccc) &&
           moku::is_nil<uint8_t>(properties.nLead) &&
           moku::is_nil<qcInfo>(properties.flags) &&
           moku::is_nil<uint16_t>(properties.index);
}
constexpr std::string Version{"9.0.0"};
bool appendFlush(reorderBuffer *rb);
int appendQuick(reorderBuffer *rb, int i);
static std::vector<uint8_t> ccc;
bool cmpNormalBytes(reorderBuffer *rb);
int32_t combine(int32_t a, int32_t b);
Properties compInfo(uint16_t v, int sz);
int decomposeHangul(moku::slice<uint8_t> buf, int32_t r);
int decomposeSegment(reorderBuffer *rb, int sp, bool atEOF);
void decomposeToLastBoundary(reorderBuffer *rb);
static std::vector<uint8_t> decomps;
moku::slice<uint8_t> doAppend(reorderBuffer *rb,
                              moku::slice<uint8_t> out,
                              int p);
moku::slice<uint8_t> doAppendInner(reorderBuffer *rb, int p);
moku::slice<uint8_t> doNormComposed(Iter *i);
moku::slice<uint8_t> doNormDecomposed(Iter *i);
static constexpr int endMulti{12128};
static moku::slice<moku::error> errs;
static constexpr int firstCCC{11422};
static constexpr int firstCCCZeroExcept{19064};
static constexpr int firstLeadingCCC{18862};
static constexpr int firstMulti{6253};
static constexpr int firstStarterWithNLead{19103};
bool flushTransform(reorderBuffer *rb);

struct formInfo {
    Form form;
    bool composing{false};
    bool compatibility{false};
    lookupFunc info;
    iterFunc nextMain;
    std::tuple<int, bool> quickSpan(input src, int i, int end, bool atEOF);
};
template <>
inline bool moku::is_nil<formInfo>(const formInfo &forminfo)
{
    return moku::is_nil<Form>(forminfo.form) &&
           moku::is_nil<bool>(forminfo.composing) &&
           moku::is_nil<bool>(forminfo.compatibility) &&
           moku::is_nil<lookupFunc>(forminfo.info) &&
           moku::is_nil<iterFunc>(forminfo.nextMain);
}
static moku::slice<formInfo *> formTable;
static constexpr int hangulBase{44032};
static constexpr int hangulBase0{234};
static constexpr int hangulBase1{176};
static constexpr int hangulBase2{128};
static constexpr int hangulEnd{55204};
static constexpr int hangulEnd0{237};
static constexpr int hangulEnd1{158};
static constexpr int hangulEnd2{164};
static constexpr int hangulUTF8Size{3};
static constexpr int headerFlagsMask{192};
static constexpr int headerLenMask{63};
static constexpr insertErr iShortDst{-1};
static constexpr insertErr iShortSrc{-2};
static constexpr insertErr iSuccess{0};

struct input {
    std::string str{""};
    moku::slice<uint8_t> bytes;
    void setBytes(moku::slice<uint8_t> str);
    void setString(std::string str);
    uint8_t _byte(int p);
    int skipASCII(int p, int max);
    int skipContinuationBytes(int p);
    moku::slice<uint8_t> appendSlice(moku::slice<uint8_t> buf, int b, int e);
    int copySlice(moku::slice<uint8_t> buf, int b, int e);
    std::tuple<uint16_t, int> charinfoNFC(int p);
    std::tuple<uint16_t, int> charinfoNFKC(int p);
    int32_t hangul(int p);
};
template <>
inline bool moku::is_nil<input>(const input &input)
{
    return moku::is_nil<std::string>(input.str) &&
           moku::is_nil<moku::slice<uint8_t>>(input.bytes);
}
input inputBytes(moku::slice<uint8_t> str);
input inputString(std::string str);

struct insertErr : public moku::basic<int> {
    insertErr() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<insertErr>(const insertErr &inserterr)
{
    return moku::is_nil<int>(int(inserterr));
}
bool isHangul(moku::slice<uint8_t> b);
bool isHangulString(std::string b);
bool isHangulWithoutJamoT(moku::slice<uint8_t> b);
bool isJamoVT(moku::slice<uint8_t> b);
} // namespace norm
