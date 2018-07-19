int maxBitStateLen(Prog *prog)
{
    if (!shouldBacktrack(prog)) {
        return 0;
    }
    return maxBacktrackVector / len(prog->Inst);
}
bitState *newBitState(Prog *prog)
{
    if (!shouldBacktrack(prog)) {
        return notBacktrack;
    }
    return &bitState{{prog, prog}};
}
bool shouldBacktrack(Prog *prog)
{
    return len(prog->Inst) <= maxBacktrackProg;
}
void bitState::reset(int end, int ncap)
{
    int visitedSize{0};

    this->end = end;
    if (cap(this->jobs) == 0) {
        this->jobs = make(moku::slice<job>, 0, 256);
    } else {
        this->jobs = moku::slice_expr<moku::slice<job>>(this->jobs, 0);
    }
    visitedSize =
            (len(this->prog->Inst) * (end + 1) + visitedBits - 1) / visitedBits;
    if (cap(this->visited) < visitedSize) {
        this->visited = make(moku::slice<uint32_t>,
                             visitedSize,
                             maxBacktrackVector / visitedBits);
    } else {
        this->visited = moku::slice_expr<moku::slice<uint32_t>>(this->visited,
                                                                visitedSize);
        {
            int i{0};
            for (i : moku::range_key<moku::slice<uint32_t>>(this->visited)) {
                this->visited[i] = 0;
            }
        }
    }
    if (cap(this->cap) < ncap) {
        this->cap = make(moku::slice<int>, ncap);
    } else {
        this->cap = moku::slice_expr<moku::slice<int>>(this->cap, ncap);
    }
    {
        int i{0};
        for (i : moku::range_key<moku::slice<int>>(this->cap)) {
            this->cap[i] = -1;
        }
    }
}
bool bitState::shouldVisit(uint32_t pc, int pos)
{
    unsigned int n{0};

    n = unsigned int(int(pc) * (this->end + 1) + pos);
    if (this->visited[n / visitedBits] & (1 << (n & (visitedBits - 1))) != 0) {
        return false;
    }
    this->visited[n / visitedBits] |= 1 << (n & (visitedBits - 1));
    return true;
}
void bitState::push(uint32_t pc, int pos, int arg)
{
    if (this->prog->Inst[pc].Op == syntax::InstFail) {
        return;
    }
    if (arg == 0 && !this->shouldVisit(pc, pos)) {
        return;
    }
    this->jobs = append(this->jobs, job{{pc, pc}, {arg, arg}, {pos, pos}});
}
bool machine::tryBacktrack(bitState *b, input i, uint32_t pc, int pos)
{
    bool longest{false};

    longest = this->re->longest;
    this->matched = false;
    b->push(pc, pos, 0);
    while (len(b->jobs) > 0) {
        int arg{0};
        Inst inst{};
        int l{0};
        uint32_t pc{0};
        int pos{0};

        l = len(b->jobs) - 1;
        pc = b->jobs[l].pc;
        pos = b->jobs[l].pos;
        arg = b->jobs[l].arg;
        b->jobs = moku::slice_expr<moku::slice<job>>(b->jobs, l);
        goto Skip;
    CheckAndLoop:;
    Skip:;
        if ((inst.Op == syntax::InstFail)) {
        _ident_1_:
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 565541) STRING "unexpected InstFail"}));
        } else if ((inst.Op == syntax::InstAlt)) {
        _ident_2_:
            if ((arg == 0)) {
            _ident_12_:
                b->push(pc, pos, 1);
                pc = inst.Out;
                goto CheckAndLoop;
            } else if ((arg == 1)) {
            _ident_13_:
                arg = 0;
                pc = inst.Arg;
                goto CheckAndLoop;
            }
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 566125) STRING "bad arg in InstAlt"}));
        } else if ((inst.Op == syntax::InstAltMatch)) {
        _ident_3_:
            if ((b->prog->Inst[inst.Out].Op == syntax::InstRune) ||
                (b->prog->Inst[inst.Out].Op == syntax::InstRune1) ||
                (b->prog->Inst[inst.Out].Op == syntax::InstRuneAny) ||
                (b->prog->Inst[inst.Out].Op == syntax::InstRuneAnyNotNL)) {
            _ident_14_:
                b->push(inst.Arg, pos, 0);
                pc = inst.Arg;
                pos = b->end;
                goto CheckAndLoop;
            }
            b->push(inst.Out, b->end, 0);
            pc = inst.Out;
            goto CheckAndLoop;
        } else if ((inst.Op == syntax::InstRune)) {
        _ident_4_:
            std::tie(r, width) = i.step(pos);
            if (!inst.MatchRune(r)) {
                continue;
            }
            pos += width;
            pc = inst.Out;
            goto CheckAndLoop;
        } else if ((inst.Op == syntax::InstRune1)) {
        _ident_5_:
            std::tie(r, width) = i.step(pos);
            if (r != inst.Rune[0]) {
                continue;
            }
            pos += width;
            pc = inst.Out;
            goto CheckAndLoop;
        } else if ((inst.Op == syntax::InstRuneAnyNotNL)) {
        _ident_6_:
            std::tie(r, width) = i.step(pos);
            if (r == '\n' || r == endOfText) {
                continue;
            }
            pos += width;
            pc = inst.Out;
            goto CheckAndLoop;
        } else if ((inst.Op == syntax::InstRuneAny)) {
        _ident_7_:
            std::tie(r, width) = i.step(pos);
            if (r == endOfText) {
                continue;
            }
            pos += width;
            pc = inst.Out;
            goto CheckAndLoop;
        } else if ((inst.Op == syntax::InstCapture)) {
        _ident_8_:
            if ((arg == 0)) {
            _ident_15_:
                if (0 <= inst.Arg && inst.Arg < uint32_t(len(b->cap))) {
                    b->push(pc, b->cap[inst.Arg], 1);
                    b->cap[inst.Arg] = pos;
                }
                pc = inst.Out;
                goto CheckAndLoop;
            } else if ((arg == 1)) {
            _ident_16_:
                b->cap[inst.Arg] = pos;
                continue;
            }
            panic(moku::make_iface<std::string>(&{
                    % !s(token.Pos = 567626) STRING "bad arg in InstCapture"}));
        } else if ((inst.Op == syntax::InstEmptyWidth)) {
        _ident_9_:
            if (syntax::EmptyOp(inst.Arg) & ~(i.context(pos)) != 0) {
                continue;
            }
            pc = inst.Out;
            goto CheckAndLoop;
        } else if ((inst.Op == syntax::InstNop)) {
        _ident_10_:
            pc = inst.Out;
            goto CheckAndLoop;
        } else if ((inst.Op == syntax::InstMatch)) {
        _ident_11_:
            if (len(b->cap) == 0) {
                this->matched = true;
                return this->matched;
            }
            if (len(b->cap) > 1) {
                b->cap[1] = pos;
            }
            if (!this->matched ||
                (longest && pos > 0 && pos > this->matchcap[1])) {
                copy(this->matchcap, b->cap);
            }
            this->matched = true;
            if (!longest) {
                return this->matched;
            }
            if (pos == b->end) {
                return this->matched;
            }
            continue;
        } else {
        _ident_0_:
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 565496) STRING "bad inst"}));
        }
    }
    return this->matched;
}
bool machine::backtrack(input i, int pos, int end, int ncap)
{
    bitState *b{std::nullptr};
    EmptyOp startCond{};
    int width{0};

    if (!i.canCheckPrefix()) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 568840) STRING
                     "backtrack called for a RuneReader"}));
    }
    startCond = this->re->cond;
    if (startCond == ^syntax::EmptyOp(0)) {
        return false;
    }
    if (startCond & syntax::EmptyBeginText != 0 && pos != 0) {
        return false;
    }
    b = this->b;
    b->reset(end, ncap);
    this->matchcap = moku::slice_expr<moku::slice<int>>(this->matchcap, ncap);
    {
        int i{0};
        for (i : moku::range_key<moku::slice<int>>(this->matchcap)) {
            this->matchcap[i] = -1;
        }
    }
    if (startCond & syntax::EmptyBeginText != 0) {
        if (len(b->cap) > 0) {
            b->cap[0] = pos;
        }
        return this->tryBacktrack(b, i, uint32_t(this->p->Start), pos);
    }
    width = -1;
    for (; pos <= end && width != 0; pos += width) {
        if (len(this->re->prefix) > 0) {
            advance = i.index(this->re, pos);
            if (advance < 0) {
                return false;
            }
            pos += advance;
        }
        if (len(b->cap) > 0) {
            b->cap[0] = pos;
        }
        if (this->tryBacktrack(b, i, uint32_t(this->p->Start), pos)) {
            return true;
        }
        std::tie(_, width) = i.step(pos);
    }
    return false;
}
void init();
int main()
{
    init();
    notBacktrack = nil;
    noRune = moku::slice<rune>{};
    noNext = moku::slice<uint32_t>{mergeFailed};
    anyRuneNotNL = moku::slice<rune>{0, '\n' - 1, '\n' + 1, unicode::MaxRune};
    anyRune = moku::slice<rune>{0, unicode::MaxRune};
    notOnePass = nil;
    _main();
    return 0;
}
