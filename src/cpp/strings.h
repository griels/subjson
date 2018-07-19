#pragma once
#include "cpu.h"
#include "errors.h"
#include "io.h"
#include "unicode.h"
#include "utf8.h"
namespace strings {
int Compare(std::string a, std::string b);
bool Contains(std::string s, std::string substr);
bool ContainsAny(std::string s, std::string chars);
bool ContainsRune(std::string s, int32_t r);
int Count(std::string s, std::string substr);
bool EqualFold(std::string s, std::string t);
moku::slice<std::string> Fields(std::string s);
moku::slice<std::string> FieldsFunc(std::string s,
                                    std::function<bool(int32_t)> f);
bool HasPrefix(std::string s, std::string prefix);
bool HasSuffix(std::string s, std::string suffix);
int Index(std::string s, std::string substr);
int IndexAny(std::string s, std::string chars);
int IndexByte(std::string s, uint8_t c);
int IndexFunc(std::string s, std::function<bool(int32_t)> f);
int IndexRune(std::string s, int32_t r);
std::string Join(moku::slice<std::string> a, std::string sep);
int LastIndex(std::string s, std::string substr);
int LastIndexAny(std::string s, std::string chars);
int LastIndexByte(std::string s, uint8_t c);
int LastIndexFunc(std::string s, std::function<bool(int32_t)> f);
std::string Map(std::function<int32_t(int32_t)> mapping, std::string s);
Reader *NewReader(std::string s);
Replacer *NewReplacer(moku::slice<std::string> oldnew);

struct Reader {
    std::string s{""};
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
    void Reset(std::string s);
};
template <>
inline bool moku::is_nil<Reader>(const Reader &reader)
{
    return moku::is_nil<std::string>(reader.s) &&
           moku::is_nil<int64_t>(reader.i) &&
           moku::is_nil<int>(reader.prevRune);
}
std::string Repeat(std::string s, int count);
std::string Replace(std::string s, std::string old, std::string new, int n);

struct Replacer {
    replacer r;
    std::string Replace(std::string s);
    std::tuple<int, moku::error> WriteString(Writer w, std::string s);
};
template <>
inline bool moku::is_nil<Replacer>(const Replacer &replacer)
{
    return moku::is_nil<replacer>(replacer.r);
}
moku::slice<std::string> Split(std::string s, std::string sep);
moku::slice<std::string> SplitAfter(std::string s, std::string sep);
moku::slice<std::string> SplitAfterN(std::string s, std::string sep, int n);
moku::slice<std::string> SplitN(std::string s, std::string sep, int n);
std::string Title(std::string s);
std::string ToLower(std::string s);
std::string ToLowerSpecial(SpecialCase c, std::string s);
std::string ToTitle(std::string s);
std::string ToTitleSpecial(SpecialCase c, std::string s);
std::string ToUpper(std::string s);
std::string ToUpperSpecial(SpecialCase c, std::string s);
std::string Trim(std::string s, std::string cutset);
std::string TrimFunc(std::string s, std::function<bool(int32_t)> f);
std::string TrimLeft(std::string s, std::string cutset);
std::string TrimLeftFunc(std::string s, std::function<bool(int32_t)> f);
std::string TrimPrefix(std::string s, std::string prefix);
std::string TrimRight(std::string s, std::string cutset);
std::string TrimRightFunc(std::string s, std::function<bool(int32_t)> f);
std::string TrimSpace(std::string s);
std::string TrimSuffix(std::string s, std::string suffix);
} // namespace strings
