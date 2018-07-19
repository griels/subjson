#pragma once
#include "bytes.h"
#include "filepath.h"
#include "io.h"
#include "os.h"
#include "sort.h"
#include "strconv.h"
#include "sync.h"
#include "time.h"
namespace ioutil {
Writer Discard;
ReadCloser NopCloser(Reader r);
std::tuple<moku::slice<uint8_t>, moku::error> ReadAll(Reader r);
std::tuple<moku::slice<FileInfo>, moku::error> ReadDir(std::string dirname);
std::tuple<moku::slice<uint8_t>, moku::error> ReadFile(std::string filename);
std::tuple<std::string, moku::error> TempDir(std::string dir,
                                             std::string prefix);
std::tuple<File *, moku::error> TempFile(std::string dir, std::string prefix);
moku::error WriteFile(std::string filename,
                      moku::slice<uint8_t> data,
                      FileMode perm);
static Pool blackHolePool;

struct devNull : public moku::basic<int> {
    devNull() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<devNull>(const devNull &devnull)
{
    return moku::is_nil<int>(int(devnull));
}
inline std::tuple<int, moku::error> Write(moku::slice<uint8_t> p)
{
    devNull devnull = *this;
    return devnull._WriteByValue(moku::slice<uint8_t> p);
}
std::tuple<int, moku::error> _WriteByValue(moku::slice<uint8_t> p);
inline std::tuple<int, moku::error> WriteString(std::string s)
{
    devNull devnull = *this;
    return devnull._WriteStringByValue(std::string s);
}
std::tuple<int, moku::error> _WriteStringByValue(std::string s);
inline std::tuple<int64_t, moku::error> ReadFrom(Reader r)
{
    devNull devnull = *this;
    return devnull._ReadFromByValue(Reader r);
}
std::tuple<int64_t, moku::error> _ReadFromByValue(Reader r);
std::string nextSuffix();

struct nopCloser {
    Reader Reader;
    inline moku::error Close()
    {
        nopCloser nopcloser = *this;
        return nopcloser._CloseByValue();
    }
    moku::error _CloseByValue();
};
template <>
inline bool moku::is_nil<nopCloser>(const nopCloser &nopcloser)
{
    return moku::is_nil<Reader>(nopcloser.Reader);
}
static uint32_t rand;
static Mutex randmu;
std::tuple<moku::slice<uint8_t>, moku::error> readAll(Reader r,
                                                      int64_t capacity);
uint32_t reseed();
} // namespace ioutil
