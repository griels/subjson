#pragma once
#include "io.h"
namespace hash {

struct Hash {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual moku::slice<uint8_t> Sum(moku::slice<uint8_t> b) = 0;
    virtual int Size() = 0;
    virtual void Reset() = 0;
    virtual int BlockSize() = 0;
};

struct Hash32 {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual uint32_t Sum32() = 0;
    virtual moku::slice<uint8_t> Sum(moku::slice<uint8_t> b) = 0;
    virtual int Size() = 0;
    virtual void Reset() = 0;
    virtual int BlockSize() = 0;
};

struct Hash64 {
    virtual std::tuple<int, moku::error> Write(moku::slice<uint8_t> p) = 0;
    virtual uint64_t Sum64() = 0;
    virtual moku::slice<uint8_t> Sum(moku::slice<uint8_t> b) = 0;
    virtual int Size() = 0;
    virtual void Reset() = 0;
    virtual int BlockSize() = 0;
};
} // namespace hash
