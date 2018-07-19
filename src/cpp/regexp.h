#pragma once
#include "bytes.h"
#include "io.h"
#include "sort.h"
#include "strconv.h"
#include "strings.h"
#include "sync.h"
#include "syntax.h"
#include "unicode.h"
#include "utf8.h"
namespace regexp {
std::tuple<Regexp *, moku::error> Compile(std::string expr);
std::tuple<Regexp *, moku::error> CompilePOSIX(std::string expr);
std::tuple<bool, moku::error> Match(std::string pattern,
                                    moku::slice<uint8_t> b);
std::tuple<bool, moku::error> MatchReader(std::string pattern, RuneReader r);
std::tuple<bool, moku::error> MatchString(std::string pattern, std::string s);
Regexp *MustCompile(std::string str);
Regexp *MustCompilePOSIX(std::string str);
std::string QuoteMeta(std::string s);

struct Regexp {
    regexpRO regexpRO;
    Mutex mu;
    moku::slice<machine *> machine;
    bool doMatch(RuneReader r, moku::slice<uint8_t> b, std::string s);
    moku::slice<int> doExecute(RuneReader r,
                               moku::slice<uint8_t> b,
                               std::string s,
                               int pos,
                               int ncap,
                               moku::slice<int> dstCap);
    std::string String();
    Regexp *Copy();
    void Longest();
    machine *get();
    void put(machine *z);
    int NumSubexp();
    moku::slice<std::string> SubexpNames();
    std::tuple<std::string, bool> LiteralPrefix();
    bool MatchReader(RuneReader r);
    bool MatchString(std::string s);
    bool Match(moku::slice<uint8_t> b);
    std::string ReplaceAllString(std::string src, std::string repl);
    std::string ReplaceAllLiteralString(std::string src, std::string repl);
    std::string ReplaceAllStringFunc(
            std::string src, std::function<std::string(std::string)> repl);
    moku::slice<uint8_t> replaceAll(
            moku::slice<uint8_t> bsrc,
            std::string src,
            int nmatch,
            std::function<moku::slice<uint8_t>(moku::slice<uint8_t>,
                                               moku::slice<int>)> repl);
    moku::slice<uint8_t> ReplaceAll(moku::slice<uint8_t> src,
                                    moku::slice<uint8_t> repl);
    moku::slice<uint8_t> ReplaceAllLiteral(moku::slice<uint8_t> src,
                                           moku::slice<uint8_t> repl);
    moku::slice<uint8_t> ReplaceAllFunc(
            moku::slice<uint8_t> src,
            std::function<moku::slice<uint8_t>(moku::slice<uint8_t>)> repl);
    moku::slice<int> pad(moku::slice<int> a);
    void allMatches(std::string s,
                    moku::slice<uint8_t> b,
                    int n,
                    std::function<void(moku::slice<int>)> deliver);
    moku::slice<uint8_t> Find(moku::slice<uint8_t> b);
    moku::slice<int> FindIndex(moku::slice<uint8_t> b);
    std::string FindString(std::string s);
    moku::slice<int> FindStringIndex(std::string s);
    moku::slice<int> FindReaderIndex(RuneReader r);
    moku::slice<moku::slice<uint8_t>> FindSubmatch(moku::slice<uint8_t> b);
    moku::slice<uint8_t> Expand(moku::slice<uint8_t> dst,
                                moku::slice<uint8_t> template,
                                moku::slice<uint8_t> src,
                                moku::slice<int> match);
    moku::slice<uint8_t> ExpandString(moku::slice<uint8_t> dst,
                                      std::string template,
                                      std::string src,
                                      moku::slice<int> match);
    moku::slice<uint8_t> expand(moku::slice<uint8_t> dst,
                                std::string template,
                                moku::slice<uint8_t> bsrc,
                                std::string src,
                                moku::slice<int> match);
    moku::slice<int> FindSubmatchIndex(moku::slice<uint8_t> b);
    moku::slice<std::string> FindStringSubmatch(std::string s);
    moku::slice<int> FindStringSubmatchIndex(std::string s);
    moku::slice<int> FindReaderSubmatchIndex(RuneReader r);
    moku::slice<moku::slice<uint8_t>> FindAll(moku::slice<uint8_t> b, int n);
    moku::slice<moku::slice<int>> FindAllIndex(moku::slice<uint8_t> b, int n);
    moku::slice<std::string> FindAllString(std::string s, int n);
    moku::slice<moku::slice<int>> FindAllStringIndex(std::string s, int n);
    moku::slice<moku::slice<moku::slice<uint8_t>>> FindAllSubmatch(
            moku::slice<uint8_t> b, int n);
    moku::slice<moku::slice<int>> FindAllSubmatchIndex(moku::slice<uint8_t> b,
                                                       int n);
    moku::slice<moku::slice<std::string>> FindAllStringSubmatch(std::string s,
                                                                int n);
    moku::slice<moku::slice<int>> FindAllStringSubmatchIndex(std::string s,
                                                             int n);
    moku::slice<std::string> Split(std::string s, int n);
};
template <>
inline bool moku::is_nil<Regexp>(const Regexp &regexp)
{
    return moku::is_nil<regexpRO>(regexp.regexpRO) &&
           moku::is_nil<Mutex>(regexp.mu) &&
           moku::is_nil<moku::slice<machine *>>(regexp.machine);
}
static moku::slice<int32_t> anyRune;
static moku::slice<int32_t> anyRuneNotNL;
static std::vector<int> arrayNoInts;

struct bitState {
    Prog *prog{std::nullptr};
    int end{0};
    moku::slice<int> cap;
    moku::slice<job> jobs;
    moku::slice<uint32_t> visited;
    void reset(int end, int ncap);
    bool shouldVisit(uint32_t pc, int pos);
    void push(uint32_t pc, int pos, int arg);
};
template <>
inline bool moku::is_nil<bitState>(const bitState &bitstate)
{
    return moku::is_nil<Prog *>(bitstate.prog) &&
           moku::is_nil<int>(bitstate.end) &&
           moku::is_nil<moku::slice<int>>(bitstate.cap) &&
           moku::is_nil<moku::slice<job>>(bitstate.jobs) &&
           moku::is_nil<moku::slice<uint32_t>>(bitstate.visited);
}
void cleanupOnePass(onePassProg *prog, Prog *original);
std::tuple<Regexp *, moku::error> compile(std::string expr,
                                          Flags mode,
                                          bool longest);
onePassProg *compileOnePass(Prog *prog);
static constexpr int32_t endOfText{-1};

struct entry {
    uint32_t pc{0};
    thread *t{std::nullptr};
};
template <>
inline bool moku::is_nil<entry>(const entry &entry)
{
    return moku::is_nil<uint32_t>(entry.pc) && moku::is_nil<thread *>(entry.t);
}
std::tuple<std::string, int, std::string, bool> extract(std::string str);

struct input {
    virtual std::tuple<int32_t, int> step(int pos) = 0;
    virtual int index(Regexp *re, int pos) = 0;
    virtual bool hasPrefix(Regexp *re) = 0;
    virtual EmptyOp context(int pos) = 0;
    virtual bool canCheckPrefix() = 0;
};

struct inputBytes {
    moku::slice<uint8_t> str;
    std::tuple<int32_t, int> step(int pos);
    bool canCheckPrefix();
    bool hasPrefix(Regexp *re);
    int index(Regexp *re, int pos);
    EmptyOp context(int pos);
};
template <>
inline bool moku::is_nil<inputBytes>(const inputBytes &inputbytes)
{
    return moku::is_nil<moku::slice<uint8_t>>(inputbytes.str);
}
struct inputReader {
    RuneReader r;
    bool atEOT{false};
    int pos{0};
    std::tuple<int32_t, int> step(int pos);
    bool canCheckPrefix();
    bool hasPrefix(Regexp *re);
    int index(Regexp *re, int pos);
    EmptyOp context(int pos);
};
template <>
inline bool moku::is_nil<inputReader>(const inputReader &inputreader)
{
    return moku::is_nil<RuneReader>(inputreader.r) &&
           moku::is_nil<bool>(inputreader.atEOT) &&
           moku::is_nil<int>(inputreader.pos);
}
struct inputString {
    std::string str{""};
    std::tuple<int32_t, int> step(int pos);
    bool canCheckPrefix();
    bool hasPrefix(Regexp *re);
    int index(Regexp *re, int pos);
    EmptyOp context(int pos);
};
template <>
inline bool moku::is_nil<inputString>(const inputString &inputstring)
{
    return moku::is_nil<std::string>(inputstring.str);
}
InstOp iop(Inst *i);

struct job {
    uint32_t pc{0};
    int arg{0};
    int pos{0};
};
template <>
inline bool moku::is_nil<job>(const job &job)
{
    return moku::is_nil<uint32_t>(job.pc) && moku::is_nil<int>(job.arg) &&
           moku::is_nil<int>(job.pos);
}
struct machine {
    Regexp *re{std::nullptr};
    Prog *p{std::nullptr};
    onePassProg *op{std::nullptr};
    int maxBitStateLen{0};
    bitState *b{std::nullptr};
    queue q0;
    queue q1;
    moku::slice<thread *> pool;
    bool matched{false};
    moku::slice<int> matchcap;
    inputBytes inputBytes;
    inputString inputString;
    inputReader inputReader;
    bool tryBacktrack(bitState *b, input i, uint32_t pc, int pos);
    bool backtrack(input i, int pos, int end, int ncap);
    input newInputBytes(moku::slice<uint8_t> b);
    input newInputString(std::string s);
    input newInputReader(RuneReader r);
    void init(int ncap);
    thread *alloc(Inst *i);
    bool match(input i, int pos);
    void clear(queue *q);
    void step(queue *runq,
              queue *nextq,
              int pos,
              int nextPos,
              int32_t c,
              EmptyOp nextCond);
    thread *add(queue *q,
                uint32_t pc,
                int pos,
                moku::slice<int> cap,
                EmptyOp cond,
                thread *t);
    bool onepass(input i, int pos, int ncap);
};
template <>
inline bool moku::is_nil<machine>(const machine &machine)
{
    return moku::is_nil<Regexp *>(machine.re) &&
           moku::is_nil<Prog *>(machine.p) &&
           moku::is_nil<onePassProg *>(machine.op) &&
           moku::is_nil<int>(machine.maxBitStateLen) &&
           moku::is_nil<bitState *>(machine.b) &&
           moku::is_nil<queue>(machine.q0) && moku::is_nil<queue>(machine.q1) &&
           moku::is_nil<moku::slice<thread *>>(machine.pool) &&
           moku::is_nil<bool>(machine.matched) &&
           moku::is_nil<moku::slice<int>>(machine.matchcap) &&
           moku::is_nil<inputBytes>(machine.inputBytes) &&
           moku::is_nil<inputString>(machine.inputString) &&
           moku::is_nil<inputReader>(machine.inputReader);
}
onePassProg *makeOnePass(onePassProg *p);
static constexpr int maxBacktrackProg{500};
static constexpr int maxBacktrackVector{262144};
int maxBitStateLen(Prog *prog);
static constexpr uint32_t mergeFailed{4294967295};
std::tuple<moku::slice<int32_t>, moku::slice<uint32_t>> mergeRuneSets(
        moku::slice<int32_t> *leftRunes,
        moku::slice<int32_t> *rightRunes,
        uint32_t leftPC,
        uint32_t rightPC);
bitState *newBitState(Prog *prog);
queueOnePass *newQueue(int size);
static moku::slice<uint32_t> noNext;
static moku::slice<int32_t> noRune;
static bitState *notBacktrack;
static onePassProg *notOnePass;
onePassProg *onePassCopy(Prog *prog);

struct onePassInst {
    Inst Inst;
    moku::slice<uint32_t> Next;
};
template <>
inline bool moku::is_nil<onePassInst>(const onePassInst &onepassinst)
{
    return moku::is_nil<Inst>(onepassinst.Inst) &&
           moku::is_nil<moku::slice<uint32_t>>(onepassinst.Next);
}
uint32_t onePassNext(onePassInst *i, int32_t r);
std::tuple<std::string, bool, uint32_t> onePassPrefix(Prog *p);

struct onePassProg {
    moku::slice<onePassInst> Inst;
    int Start{0};
    int NumCap{0};
};
template <>
inline bool moku::is_nil<onePassProg>(const onePassProg &onepassprog)
{
    return moku::is_nil<moku::slice<onePassInst>>(onepassprog.Inst) &&
           moku::is_nil<int>(onepassprog.Start) &&
           moku::is_nil<int>(onepassprog.NumCap);
}
machine *progMachine(Prog *p, onePassProg *op);

struct queue {
    moku::slice<uint32_t> sparse;
    moku::slice<entry> dense;
};
template <>
inline bool moku::is_nil<queue>(const queue &queue)
{
    return moku::is_nil<moku::slice<uint32_t>>(queue.sparse) &&
           moku::is_nil<moku::slice<entry>>(queue.dense);
}
struct queueOnePass {
    moku::slice<uint32_t> sparse;
    moku::slice<uint32_t> dense;
    uint32_t size{0};
    uint32_t nextIndex{0};
    bool empty();
    uint32_t next();
    void clear();
    bool contains(uint32_t u);
    void insert(uint32_t u);
    void insertNew(uint32_t u);
};
template <>
inline bool moku::is_nil<queueOnePass>(const queueOnePass &queueonepass)
{
    return moku::is_nil<moku::slice<uint32_t>>(queueonepass.sparse) &&
           moku::is_nil<moku::slice<uint32_t>>(queueonepass.dense) &&
           moku::is_nil<uint32_t>(queueonepass.size) &&
           moku::is_nil<uint32_t>(queueonepass.nextIndex);
}
std::string quote(std::string s);

struct regexpRO {
    std::string expr{""};
    Prog *prog{std::nullptr};
    onePassProg *onepass{std::nullptr};
    std::string prefix{""};
    moku::slice<uint8_t> prefixBytes;
    bool prefixComplete{false};
    int32_t prefixRune{0};
    uint32_t prefixEnd{0};
    EmptyOp cond;
    int numSubexp{0};
    moku::slice<std::string> subexpNames;
    bool longest{false};
};
template <>
inline bool moku::is_nil<regexpRO>(const regexpRO &regexpro)
{
    return moku::is_nil<std::string>(regexpro.expr) &&
           moku::is_nil<Prog *>(regexpro.prog) &&
           moku::is_nil<onePassProg *>(regexpro.onepass) &&
           moku::is_nil<std::string>(regexpro.prefix) &&
           moku::is_nil<moku::slice<uint8_t>>(regexpro.prefixBytes) &&
           moku::is_nil<bool>(regexpro.prefixComplete) &&
           moku::is_nil<int32_t>(regexpro.prefixRune) &&
           moku::is_nil<uint32_t>(regexpro.prefixEnd) &&
           moku::is_nil<EmptyOp>(regexpro.cond) &&
           moku::is_nil<int>(regexpro.numSubexp) &&
           moku::is_nil<moku::slice<std::string>>(regexpro.subexpNames) &&
           moku::is_nil<bool>(regexpro.longest);
}
} // namespace regexp
