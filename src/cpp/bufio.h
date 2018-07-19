#pragma once
#include "bytes.h"
#include "errors.h"
#include "io.h"
#include "utf8.h"
namespace bufio {
moku::error ErrAdvanceTooFar;
moku::error ErrBufferFull;
moku::error ErrFinalToken;
moku::error ErrInvalidUnreadByte;
moku::error ErrInvalidUnreadRune;
moku::error ErrNegativeAdvance;
moku::error ErrNegativeCount;
moku::error ErrTooLong;
constexpr int MaxScanTokenSize{65536};
ReadWriter *NewReadWriter(Reader *r, Writer *w);
Reader *NewReader(Reader rd);
Reader *NewReaderSize(Reader rd, int size);
Scanner *NewScanner(Reader r);
Writer *NewWriter(Writer w);
Writer *NewWriterSize(Writer w, int size);

struct ReadWriter {
    Reader *Reader{std::nullptr};
    Writer *Writer{std::nullptr};
};
template <>
inline bool moku::is_nil<ReadWriter>(const ReadWriter &readwriter)
{
    return moku::is_nil<Reader *>(readwriter.Reader) &&
           moku::is_nil<Writer *>(readwriter.Writer);
}
struct Reader {
    moku::slice<uint8_t> buf;
    Reader rd;
    int r{0};
    int w{0};
    moku::error err;
    int lastByte{0};
    int lastRuneSize{0};
    void Reset(Reader r);
    void reset(moku::slice<uint8_t> buf, Reader r);
    void fill();
    moku::error readErr();
    std::tuple<moku::slice<uint8_t>, moku::error> Peek(int n);
    std::tuple<int, moku::error> Discard(int n);
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
    std::tuple<uint8_t, moku::error> ReadByte();
    moku::error UnreadByte();
    std::tuple<int32_t, int, moku::error> ReadRune();
    moku::error UnreadRune();
    int Buffered();
    std::tuple<moku::slice<uint8_t>, moku::error> ReadSlice(uint8_t delim);
    std::tuple<moku::slice<uint8_t>, bool, moku::error> ReadLine();
    std::tuple<moku::slice<uint8_t>, moku::error> ReadBytes(uint8_t delim);
    std::tuple<std::string, moku::error> ReadString(uint8_t delim);
    std::tuple<int64_t, moku::error> WriteTo(Writer w);
    std::tuple<int64_t, moku::error> writeBuf(Writer w);
};
template <>
inline bool moku::is_nil<Reader>(const Reader &reader)
{
    return moku::is_nil<moku::slice<uint8_t>>(reader.buf) &&
           moku::is_nil<Reader>(reader.rd) && moku::is_nil<int>(reader.r) &&
           moku::is_nil<int>(reader.w) &&
           moku::is_nil<moku::error>(reader.err) &&
           moku::is_nil<int>(reader.lastByte) &&
           moku::is_nil<int>(reader.lastRuneSize);
}
std::tuple<int, moku::slice<uint8_t>, moku::error> ScanBytes(
        moku::slice<uint8_t> data, bool atEOF);
std::tuple<int, moku::slice<uint8_t>, moku::error> ScanLines(
        moku::slice<uint8_t> data, bool atEOF);
std::tuple<int, moku::slice<uint8_t>, moku::error> ScanRunes(
        moku::slice<uint8_t> data, bool atEOF);
std::tuple<int, moku::slice<uint8_t>, moku::error> ScanWords(
        moku::slice<uint8_t> data, bool atEOF);

struct Scanner {
    Reader r;
    SplitFunc split;
    int maxTokenSize{0};
    moku::slice<uint8_t> token;
    moku::slice<uint8_t> buf;
    int start{0};
    int end{0};
    moku::error err;
    int empties{0};
    bool scanCalled{false};
    bool done{false};
    moku::error Err();
    moku::slice<uint8_t> Bytes();
    std::string Text();
    bool Scan();
    bool advance(int n);
    void setErr(moku::error err);
    void Buffer(moku::slice<uint8_t> buf, int max);
    void Split(SplitFunc split);
};
template <>
inline bool moku::is_nil<Scanner>(const Scanner &scanner)
{
    return moku::is_nil<Reader>(scanner.r) &&
           moku::is_nil<SplitFunc>(scanner.split) &&
           moku::is_nil<int>(scanner.maxTokenSize) &&
           moku::is_nil<moku::slice<uint8_t>>(scanner.token) &&
           moku::is_nil<moku::slice<uint8_t>>(scanner.buf) &&
           moku::is_nil<int>(scanner.start) && moku::is_nil<int>(scanner.end) &&
           moku::is_nil<moku::error>(scanner.err) &&
           moku::is_nil<int>(scanner.empties) &&
           moku::is_nil<bool>(scanner.scanCalled) &&
           moku::is_nil<bool>(scanner.done);
}
} // namespace bufio
