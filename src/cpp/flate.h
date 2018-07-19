#pragma once
#include "bits.h"
#include "bufio.h"
#include "fmt.h"
#include "io.h"
#include "math.h"
#include "sort.h"
#include "strconv.h"
#include "sync.h"
namespace flate {
constexpr int BestCompression{9};
constexpr int BestSpeed{1};

struct CorruptInputError : public moku::basic<int64_t> {
    CorruptInputError() : moku::basic<int64_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<CorruptInputError>(
        const CorruptInputError &corruptinputerror)
{
    return moku::is_nil<int64_t>(int64_t(corruptinputerror));
}
inline std::string Error()
{
    CorruptInputError corruptinputerror = *this;
    return corruptinputerror._ErrorByValue();
}
std::string _ErrorByValue();
constexpr int DefaultCompression{-1};
constexpr int HuffmanOnly{-2};

struct InternalError : public moku::basic<std::string> {
    InternalError() : moku::basic<std::string>{""}
    {
    }
};
template <>
inline bool moku::is_nil<InternalError>(const InternalError &internalerror)
{
    return moku::is_nil<std::string>(std::string(internalerror));
}
inline std::string Error()
{
    InternalError internalerror = *this;
    return internalerror._ErrorByValue();
}
std::string _ErrorByValue();
ReadCloser NewReader(Reader r);
ReadCloser NewReaderDict(Reader r, moku::slice<uint8_t> dict);
std::tuple<Writer *, moku::error> NewWriter(Writer w, int level);
std::tuple<Writer *, moku::error> NewWriterDict(Writer w,
                                                int level,
                                                moku::slice<uint8_t> dict);
constexpr int NoCompression{0};

struct ReadError {
    int64_t Offset{0};
    moku::error Err;
    std::string Error();
};
template <>
inline bool moku::is_nil<ReadError>(const ReadError &readerror)
{
    return moku::is_nil<int64_t>(readerror.Offset) &&
           moku::is_nil<moku::error>(readerror.Err);
}
struct Reader {
    virtual std::tuple<uint8_t, moku::error> ReadByte() = 0;
    virtual std::tuple<int, moku::error> Read(moku::slice<uint8_t> p) = 0;
};

struct Resetter {
    virtual moku::error Reset(Reader r, moku::slice<uint8_t> dict) = 0;
};

struct WriteError {
    int64_t Offset{0};
    moku::error Err;
    std::string Error();
};
template <>
inline bool moku::is_nil<WriteError>(const WriteError &writeerror)
{
    return moku::is_nil<int64_t>(writeerror.Offset) &&
           moku::is_nil<moku::error>(writeerror.Err);
}
struct Writer {
    compressor d;
    moku::slice<uint8_t> dict;
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> data);
    moku::error Flush();
    moku::error Close();
    void Reset(Writer dst);
};
template <>
inline bool moku::is_nil<Writer>(const Writer &writer)
{
    return moku::is_nil<compressor>(writer.d) &&
           moku::is_nil<moku::slice<uint8_t>>(writer.dict);
}
static constexpr int badCode{255};
static constexpr int baseMatchLength{3};
static constexpr int baseMatchOffset{1};
static constexpr int bufferFlushSize{240};
static constexpr int bufferSize{248};
void bulkHash4(moku::slice<uint8_t> b, moku::slice<uint32_t> dst);
} // namespace flate
