#pragma once
#include "cpu.h"
#include "errors.h"
#include "io.h"
#include "unicode.h"
#include "utf8.h"
namespace bytes {

struct Buffer {
    moku::slice<uint8_t> buf;
    int off{0};
    readOp lastRead;
    std::vector<uint8_t> bootstrap;
    moku::slice<uint8_t> Bytes();
    std::string String();
    int Len();
    int Cap();
    void Truncate(int n);
    void Reset();
    std::tuple<int, bool> tryGrowByReslice(int n);
    int grow(int n);
    void Grow(int n);
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> p);
    std::tuple<int, moku::error> WriteString(std::string s);
    std::tuple<int64_t, moku::error> ReadFrom(Reader r);
    std::tuple<int64_t, moku::error> WriteTo(Writer w);
    moku::error WriteByte(uint8_t c);
    std::tuple<int, moku::error> WriteRune(int32_t r);
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
    moku::slice<uint8_t> Next(int n);
    std::tuple<uint8_t, moku::error> ReadByte();
    std::tuple<int32_t, int, moku::error> ReadRune();
    moku::error UnreadRune();
    moku::error UnreadByte();
    std::tuple<moku::slice<uint8_t>, moku::error> ReadBytes(uint8_t delim);
    std::tuple<moku::slice<uint8_t>, moku::error> readSlice(uint8_t delim);
    std::tuple<std::string, moku::error> ReadString(uint8_t delim);
};
template <>
inline bool moku::is_nil<Buffer>(const Buffer &buffer)
{
    return moku::is_nil<moku::slice<uint8_t>>(buffer.buf) &&
           moku::is_nil<int>(buffer.off) &&
           moku::is_nil<readOp>(buffer.lastRead) &&
           moku::is_nil<std::vector<uint8_t>>(buffer.bootstrap);
}
int Compare(moku::slice<uint8_t> a, moku::slice<uint8_t> b);
bool Contains(moku::slice<uint8_t> b, moku::slice<uint8_t> subslice);
bool ContainsAny(moku::slice<uint8_t> b, std::string chars);
bool ContainsRune(moku::slice<uint8_t> b, int32_t r);
int Count(moku::slice<uint8_t> s, moku::slice<uint8_t> sep);
bool Equal(moku::slice<uint8_t> a, moku::slice<uint8_t> b);
bool EqualFold(moku::slice<uint8_t> s, moku::slice<uint8_t> t);
moku::error ErrTooLarge;
moku::slice<moku::slice<uint8_t>> Fields(moku::slice<uint8_t> s);
moku::slice<moku::slice<uint8_t>> FieldsFunc(moku::slice<uint8_t> s,
                                             std::function<bool(int32_t)> f);
bool HasPrefix(moku::slice<uint8_t> s, moku::slice<uint8_t> prefix);
bool HasSuffix(moku::slice<uint8_t> s, moku::slice<uint8_t> suffix);
int Index(moku::slice<uint8_t> s, moku::slice<uint8_t> sep);
int IndexAny(moku::slice<uint8_t> s, std::string chars);
int IndexByte(moku::slice<uint8_t> s, uint8_t c);
int IndexFunc(moku::slice<uint8_t> s, std::function<bool(int32_t)> f);
int IndexRune(moku::slice<uint8_t> s, int32_t r);
moku::slice<uint8_t> Join(moku::slice<moku::slice<uint8_t>> s,
                          moku::slice<uint8_t> sep);
int LastIndex(moku::slice<uint8_t> s, moku::slice<uint8_t> sep);
int LastIndexAny(moku::slice<uint8_t> s, std::string chars);
int LastIndexByte(moku::slice<uint8_t> s, uint8_t c);
int LastIndexFunc(moku::slice<uint8_t> s, std::function<bool(int32_t)> f);
moku::slice<uint8_t> Map(std::function<int32_t(int32_t)> mapping,
                         moku::slice<uint8_t> s);
constexpr int MinRead{512};
Buffer *NewBuffer(moku::slice<uint8_t> buf);
Buffer *NewBufferString(std::string s);
Reader *NewReader(moku::slice<uint8_t> b);

struct Reader {
    moku::slice<uint8_t> s;
    int64_t i{0};
    int prevRune{0};
    int Len();
    int64_t Size();
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> b);
    std::tuple<int, moku::error> ReadAt(moku::slice<uint8_t> b, int64_t off);
    std::tuple<uint8_t, moku::error> ReadByte();
    moku::error UnreadByte();
    std::tuple<int32_t, int, moku::error> ReadRune();
    moku::error UnreadRune();
    std::tuple<int64_t, moku::error> Seek(int64_t offset, int whence);
    std::tuple<int64_t, moku::error> WriteTo(Writer w);
    void Reset(moku::slice<uint8_t> b);
};
template <>
inline bool moku::is_nil<Reader>(const Reader &reader)
{
    return moku::is_nil<moku::slice<uint8_t>>(reader.s) &&
           moku::is_nil<int64_t>(reader.i) &&
           moku::is_nil<int>(reader.prevRune);
}
moku::slice<uint8_t> Repeat(moku::slice<uint8_t> b, int count);
moku::slice<uint8_t> Replace(moku::slice<uint8_t> s,
                             moku::slice<uint8_t> old,
                             moku::slice<uint8_t> new,
                             int n);
moku::slice<int32_t> Runes(moku::slice<uint8_t> s);
moku::slice<moku::slice<uint8_t>> Split(moku::slice<uint8_t> s,
                                        moku::slice<uint8_t> sep);
moku::slice<moku::slice<uint8_t>> SplitAfter(moku::slice<uint8_t> s,
                                             moku::slice<uint8_t> sep);
moku::slice<moku::slice<uint8_t>> SplitAfterN(moku::slice<uint8_t> s,
                                              moku::slice<uint8_t> sep,
                                              int n);
moku::slice<moku::slice<uint8_t>> SplitN(moku::slice<uint8_t> s,
                                         moku::slice<uint8_t> sep,
                                         int n);
moku::slice<uint8_t> Title(moku::slice<uint8_t> s);
moku::slice<uint8_t> ToLower(moku::slice<uint8_t> s);
moku::slice<uint8_t> ToLowerSpecial(SpecialCase c, moku::slice<uint8_t> s);
moku::slice<uint8_t> ToTitle(moku::slice<uint8_t> s);
moku::slice<uint8_t> ToTitleSpecial(SpecialCase c, moku::slice<uint8_t> s);
moku::slice<uint8_t> ToUpper(moku::slice<uint8_t> s);
moku::slice<uint8_t> ToUpperSpecial(SpecialCase c, moku::slice<uint8_t> s);
moku::slice<uint8_t> Trim(moku::slice<uint8_t> s, std::string cutset);
moku::slice<uint8_t> TrimFunc(moku::slice<uint8_t> s,
                              std::function<bool(int32_t)> f);
moku::slice<uint8_t> TrimLeft(moku::slice<uint8_t> s, std::string cutset);
moku::slice<uint8_t> TrimLeftFunc(moku::slice<uint8_t> s,
                                  std::function<bool(int32_t)> f);
moku::slice<uint8_t> TrimPrefix(moku::slice<uint8_t> s,
                                moku::slice<uint8_t> prefix);
moku::slice<uint8_t> TrimRight(moku::slice<uint8_t> s, std::string cutset);
moku::slice<uint8_t> TrimRightFunc(moku::slice<uint8_t> s,
                                   std::function<bool(int32_t)> f);
moku::slice<uint8_t> TrimSpace(moku::slice<uint8_t> s);
moku::slice<uint8_t> TrimSuffix(moku::slice<uint8_t> s,
                                moku::slice<uint8_t> suffix);
} // namespace bytes
