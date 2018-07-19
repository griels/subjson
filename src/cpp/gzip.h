#pragma once
#include "binary.h"
#include "bufio.h"
#include "crc32.h"
#include "errors.h"
#include "flate.h"
#include "fmt.h"
#include "io.h"
#include "time.h"
namespace gzip {
constexpr int BestCompression{9};
constexpr int BestSpeed{1};
constexpr int DefaultCompression{-1};
moku::error ErrChecksum;
moku::error ErrHeader;

struct Header {
    std::string Comment{""};
    moku::slice<uint8_t> Extra;
    Time ModTime;
    std::string Name{""};
    uint8_t OS{0};
};
template <>
inline bool moku::is_nil<Header>(const Header &header)
{
    return moku::is_nil<std::string>(header.Comment) &&
           moku::is_nil<moku::slice<uint8_t>>(header.Extra) &&
           moku::is_nil<Time>(header.ModTime) &&
           moku::is_nil<std::string>(header.Name) &&
           moku::is_nil<uint8_t>(header.OS);
}
constexpr int HuffmanOnly{-2};
std::tuple<Reader *, moku::error> NewReader(Reader r);
Writer *NewWriter(Writer w);
std::tuple<Writer *, moku::error> NewWriterLevel(Writer w, int level);
constexpr int NoCompression{0};

struct Reader {
    Header Header;
    Reader r;
    ReadCloser decompressor;
    uint32_t digest{0};
    uint32_t size{0};
    std::vector<uint8_t> buf;
    moku::error err;
    bool multistream{false};
    moku::error Reset(Reader r);
    void Multistream(bool ok);
    std::tuple<std::string, moku::error> readString();
    std::tuple<Header, moku::error> readHeader();
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
    moku::error Close();
};
template <>
inline bool moku::is_nil<Reader>(const Reader &reader)
{
    return moku::is_nil<Header>(reader.Header) &&
           moku::is_nil<Reader>(reader.r) &&
           moku::is_nil<ReadCloser>(reader.decompressor) &&
           moku::is_nil<uint32_t>(reader.digest) &&
           moku::is_nil<uint32_t>(reader.size) &&
           moku::is_nil<std::vector<uint8_t>>(reader.buf) &&
           moku::is_nil<moku::error>(reader.err) &&
           moku::is_nil<bool>(reader.multistream);
}
struct Writer {
    Header Header;
    Writer w;
    int level{0};
    bool wroteHeader{false};
    Writer *compressor{std::nullptr};
    uint32_t digest{0};
    uint32_t size{0};
    bool closed{false};
    std::vector<uint8_t> buf;
    moku::error err;
    void init(Writer w, int level);
    void Reset(Writer w);
    moku::error writeBytes(moku::slice<uint8_t> b);
    moku::error writeString(std::string s);
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> p);
    moku::error Flush();
    moku::error Close();
};
template <>
inline bool moku::is_nil<Writer>(const Writer &writer)
{
    return moku::is_nil<Header>(writer.Header) &&
           moku::is_nil<Writer>(writer.w) && moku::is_nil<int>(writer.level) &&
           moku::is_nil<bool>(writer.wroteHeader) &&
           moku::is_nil<Writer *>(writer.compressor) &&
           moku::is_nil<uint32_t>(writer.digest) &&
           moku::is_nil<uint32_t>(writer.size) &&
           moku::is_nil<bool>(writer.closed) &&
           moku::is_nil<std::vector<uint8_t>>(writer.buf) &&
           moku::is_nil<moku::error>(writer.err);
}
static constexpr int flagComment{16};
static constexpr int flagExtra{4};
static constexpr int flagHdrCrc{2};
static constexpr int flagName{8};
static constexpr int flagText{1};
static constexpr int gzipDeflate{8};
static constexpr int gzipID1{31};
static constexpr int gzipID2{139};
static littleEndian le;
moku::error noEOF(moku::error err);
} // namespace gzip
