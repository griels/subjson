#pragma once
namespace encoding {

struct BinaryMarshaler {
    virtual std::tuple<moku::slice<uint8_t>, moku::error> MarshalBinary() = 0;
};

struct BinaryUnmarshaler {
    virtual moku::error UnmarshalBinary(moku::slice<uint8_t> data) = 0;
};

struct TextMarshaler {
    virtual std::tuple<moku::slice<uint8_t>, moku::error> MarshalText() = 0;
};

struct TextUnmarshaler {
    virtual moku::error UnmarshalText(moku::slice<uint8_t> text) = 0;
};
} // namespace encoding
