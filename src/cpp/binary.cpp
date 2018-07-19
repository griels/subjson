uint16_t littleEndian::_Uint16ByValue(moku::slice<uint8_t> b)
{
    _ = b[1];
    return uint16_t(b[0]) | uint16_t(b[1]) << 8;
}
void littleEndian::_PutUint16ByValue(moku::slice<uint8_t> b, uint16_t v)
{
    _ = b[1];
    b[0] = byte(v);
    b[1] = byte(v >> 8);
}
uint32_t littleEndian::_Uint32ByValue(moku::slice<uint8_t> b)
{
    _ = b[3];
    return uint32_t(b[0]) | uint32_t(b[1]) << 8 | uint32_t(b[2]) << 16 |
           uint32_t(b[3]) << 24;
}
void littleEndian::_PutUint32ByValue(moku::slice<uint8_t> b, uint32_t v)
{
    _ = b[3];
    b[0] = byte(v);
    b[1] = byte(v >> 8);
    b[2] = byte(v >> 16);
    b[3] = byte(v >> 24);
}
uint64_t littleEndian::_Uint64ByValue(moku::slice<uint8_t> b)
{
    _ = b[7];
    return uint64_t(b[0]) | uint64_t(b[1]) << 8 | uint64_t(b[2]) << 16 |
           uint64_t(b[3]) << 24 | uint64_t(b[4]) << 32 | uint64_t(b[5]) << 40 |
           uint64_t(b[6]) << 48 | uint64_t(b[7]) << 56;
}
void littleEndian::_PutUint64ByValue(moku::slice<uint8_t> b, uint64_t v)
{
    _ = b[7];
    b[0] = byte(v);
    b[1] = byte(v >> 8);
    b[2] = byte(v >> 16);
    b[3] = byte(v >> 24);
    b[4] = byte(v >> 32);
    b[5] = byte(v >> 40);
    b[6] = byte(v >> 48);
    b[7] = byte(v >> 56);
}
std::string littleEndian::_StringByValue()
{
    return "LittleEndian";
}
std::string littleEndian::_GoStringByValue()
{
    return "binary.LittleEndian";
}
uint16_t bigEndian::_Uint16ByValue(moku::slice<uint8_t> b)
{
    _ = b[1];
    return uint16_t(b[1]) | uint16_t(b[0]) << 8;
}
void bigEndian::_PutUint16ByValue(moku::slice<uint8_t> b, uint16_t v)
{
    _ = b[1];
    b[0] = byte(v >> 8);
    b[1] = byte(v);
}
uint32_t bigEndian::_Uint32ByValue(moku::slice<uint8_t> b)
{
    _ = b[3];
    return uint32_t(b[3]) | uint32_t(b[2]) << 8 | uint32_t(b[1]) << 16 |
           uint32_t(b[0]) << 24;
}
void bigEndian::_PutUint32ByValue(moku::slice<uint8_t> b, uint32_t v)
{
    _ = b[3];
    b[0] = byte(v >> 24);
    b[1] = byte(v >> 16);
    b[2] = byte(v >> 8);
    b[3] = byte(v);
}
uint64_t bigEndian::_Uint64ByValue(moku::slice<uint8_t> b)
{
    _ = b[7];
    return uint64_t(b[7]) | uint64_t(b[6]) << 8 | uint64_t(b[5]) << 16 |
           uint64_t(b[4]) << 24 | uint64_t(b[3]) << 32 | uint64_t(b[2]) << 40 |
           uint64_t(b[1]) << 48 | uint64_t(b[0]) << 56;
}
void bigEndian::_PutUint64ByValue(moku::slice<uint8_t> b, uint64_t v)
{
    _ = b[7];
    b[0] = byte(v >> 56);
    b[1] = byte(v >> 48);
    b[2] = byte(v >> 40);
    b[3] = byte(v >> 32);
    b[4] = byte(v >> 24);
    b[5] = byte(v >> 16);
    b[6] = byte(v >> 8);
    b[7] = byte(v);
}
std::string bigEndian::_StringByValue()
{
    return "BigEndian";
}
std::string bigEndian::_GoStringByValue()
{
    return "binary.BigEndian";
}
moku::error Read(Reader r, ByteOrder order, moku::interface data)
{
}
