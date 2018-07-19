#pragma once
#include "errors.h"
#include "sync.h"
namespace io {

struct ByteReader {
    virtual std::tuple<uint8_t, moku::error> ReadByte() = 0;
};

struct ByteScanner {
    virtual moku::error UnreadByte() = 0;
    virtual std::tuple<uint8_t, moku::error> ReadByte() = 0;
};

struct ByteWriter {
    virtual moku::error WriteByte(uint8_t c) = 0;
};

struct Closer {
    virtual moku::error Close() = 0;
};
std::tuple<int64_t, moku::error> Copy(Writer dst, Reader src);
std::tuple<int64_t, moku::error> CopyBuffer(Writer dst,
                                            Reader src,
                                            moku::slice<uint8_t> buf);
std::tuple<int64_t, moku::error> CopyN(Writer dst, Reader src, int64_t n);
moku::error EOF;
moku::error ErrClosedPipe;
moku::error ErrNoProgress;
moku::error ErrShortBuffer;
moku::error ErrShortWrite;
moku::error ErrUnexpectedEOF;
Reader LimitReader(Reader r, int64_t n);

struct LimitedReader {
    Reader R;
    int64_t N{0};
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
};
template <>
inline bool moku::is_nil<LimitedReader>(const LimitedReader &limitedreader)
{
    return moku::is_nil<Reader>(limitedreader.R) &&
           moku::is_nil<int64_t>(limitedreader.N);
}
Reader MultiReader(moku::slice<Reader> readers);
Writer MultiWriter(moku::slice<Writer> writers);
SectionReader *NewSectionReader(ReaderAt r, int64_t off, int64_t n);
std::tuple<PipeReader *, PipeWriter *> Pipe();

struct PipeReader {
    pipe *p{std::nullptr};
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> data);
    moku::error Close();
    moku::error CloseWithError(moku::error err);
};
template <>
inline bool moku::is_nil<PipeReader>(const PipeReader &pipereader)
{
    return moku::is_nil<pipe *>(pipereader.p);
}
struct PipeWriter {
    pipe *p{std::nullptr};
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> data);
    moku::error Close();
    moku::error CloseWithError(moku::error err);
};
template <>
inline bool moku::is_nil<PipeWriter>(const PipeWriter &pipewriter)
{
    return moku::is_nil<pipe *>(pipewriter.p);
}
std::tuple<int, moku::error> ReadAtLeast(Reader r,
                                         moku::slice<uint8_t> buf,
                                         int min);

struct ReadCloser {
    virtual std::tuple<int, moku::error> Read(moku::slice<uint8_t> p) = 0;
    virtual moku::error Close() = 0;
};
template <>
inline eofReader *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<eofReader>(iface);
}
std::tuple<int, moku::error> ReadFull(Reader r, moku::slice<uint8_t> buf);

struct ReadSeeker {
    virtual std::tuple<int64_t, moku::error> Seek(int64_t offset,
                                                  int whence) = 0;
    virtual std::tuple<int, moku::error> Read(moku::slice<uint8_t> p) = 0;
};

struct ReadWriteCloser {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual std::tuple<int, moku::error> Read(moku::slice<uint8_t> p) = 0;
    virtual moku::error Close() = 0;
};

struct ReadWriteSeeker {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual std::tuple<int64_t, moku::error> Seek(int64_t offset,
                                                  int whence) = 0;
    virtual std::tuple<int, moku::error> Read(moku::slice<uint8_t> p) = 0;
};

struct ReadWriter {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual std::tuple<int, moku::error> Read(moku::slice<uint8_t> p) = 0;
};

struct Reader {
    virtual std::tuple<int, moku::error> Read(moku::slice<uint8_t> p) = 0;
};

struct ReaderAt {
    virtual std::tuple<int, moku::error> ReadAt(moku::slice<uint8_t> p,
                                                int64_t off) = 0;
};

struct ReaderFrom {
    virtual std::tuple<int64_t, moku::error> ReadFrom(Reader r) = 0;
};

struct RuneReader {
    virtual std::tuple<int32_t, int, moku::error> ReadRune() = 0;
};

struct RuneScanner {
    virtual moku::error UnreadRune() = 0;
    virtual std::tuple<int32_t, int, moku::error> ReadRune() = 0;
};

struct SectionReader {
    ReaderAt r;
    int64_t base{0};
    int64_t off{0};
    int64_t limit{0};
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
    std::tuple<int64_t, moku::error> Seek(int64_t offset, int whence);
    std::tuple<int, moku::error> ReadAt(moku::slice<uint8_t> p, int64_t off);
    int64_t Size();
};
template <>
inline bool moku::is_nil<SectionReader>(const SectionReader &sectionreader)
{
    return moku::is_nil<ReaderAt>(sectionreader.r) &&
           moku::is_nil<int64_t>(sectionreader.base) &&
           moku::is_nil<int64_t>(sectionreader.off) &&
           moku::is_nil<int64_t>(sectionreader.limit);
}
constexpr int SeekCurrent{1};
constexpr int SeekEnd{2};
constexpr int SeekStart{0};

struct Seeker {
    virtual std::tuple<int64_t, moku::error> Seek(int64_t offset,
                                                  int whence) = 0;
};
Reader TeeReader(Reader r, Writer w);

struct WriteCloser {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual moku::error Close() = 0;
};

struct WriteSeeker {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual std::tuple<int64_t, moku::error> Seek(int64_t offset,
                                                  int whence) = 0;
};
std::tuple<int, moku::error> WriteString(Writer w, std::string s);

struct Writer {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
};

struct WriterAt {
    virtual std::tuple<int, moku::error> WriteAt(moku::slice<uint8_t> p,
                                                 int64_t off) = 0;
};

struct WriterTo {
    virtual std::tuple<int64_t, moku::error> WriteTo(Writer w) = 0;
};
std::tuple<int64_t, moku::error> copyBuffer(Writer dst,
                                            Reader src,
                                            moku::slice<uint8_t> buf);

struct eofReader : public ReadCloser {
    inline virtual std::tuple<int, moku::error> Read(
            moku::slice<uint8_t>) override
    {
        eofReader eofreader = *this;
        return eofreader._ReadByValue(moku::slice<uint8_t>);
    }
    std::tuple<int, moku::error> _ReadByValue(moku::slice<uint8_t>);
};
template <>
inline bool moku::is_nil<eofReader>(const eofReader &eofreader)
{
    return;
}
template <>
eofReader *moku::try_assert(const moku::interface &iface)
{
    if (ReadCloser *readcloser =
                moku::type_registry::try_assert<ReadCloser>(iface))
        return readcloser;
    return std::nullptr;
}
static moku::error errOffset;
static moku::error errWhence;

struct multiReader {
    moku::slice<Reader> readers;
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
};
template <>
inline bool moku::is_nil<multiReader>(const multiReader &multireader)
{
    return moku::is_nil<moku::slice<Reader>>(multireader.readers);
}
struct multiWriter {
    moku::slice<Writer> writers;
    std::tuple<int, moku::error> Write(moku::slice<uint8_t> p);
    std::tuple<int, moku::error> WriteString(std::string s);
};
template <>
inline bool moku::is_nil<multiWriter>(const multiWriter &multiwriter)
{
    return moku::is_nil<moku::slice<Writer>>(multiwriter.writers);
}
struct pipe {
    Mutex rl;
    Mutex wl;
    Mutex l;
    moku::slice<uint8_t> data;
    Cond rwait;
    Cond wwait;
    moku::error rerr;
    moku::error werr;
    std::tuple<int, moku::error> read(moku::slice<uint8_t> b);
    std::tuple<int, moku::error> write(moku::slice<uint8_t> b);
    void rclose(moku::error err);
    void wclose(moku::error err);
};
template <>
inline bool moku::is_nil<pipe>(const pipe &pipe)
{
    return moku::is_nil<Mutex>(pipe.rl) && moku::is_nil<Mutex>(pipe.wl) &&
           moku::is_nil<Mutex>(pipe.l) &&
           moku::is_nil<moku::slice<uint8_t>>(pipe.data) &&
           moku::is_nil<Cond>(pipe.rwait) && moku::is_nil<Cond>(pipe.wwait) &&
           moku::is_nil<moku::error>(pipe.rerr) &&
           moku::is_nil<moku::error>(pipe.werr);
}
struct stringWriter {
    virtual std::tuple<int, moku::error> WriteString(std::string s) = 0;
};

struct teeReader {
    Reader r;
    Writer w;
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> p);
};
template <>
inline bool moku::is_nil<teeReader>(const teeReader &teereader)
{
    return moku::is_nil<Reader>(teereader.r) &&
           moku::is_nil<Writer>(teereader.w);
}
static std::vector<uint8_t> zero;
} // namespace io
