#pragma once
namespace subtle {
int ConstantTimeByteEq(uint8_t x, uint8_t y);
int ConstantTimeCompare(moku::slice<uint8_t> x, moku::slice<uint8_t> y);
void ConstantTimeCopy(int v, moku::slice<uint8_t> x, moku::slice<uint8_t> y);
int ConstantTimeEq(int32_t x, int32_t y);
int ConstantTimeLessOrEq(int x, int y);
int ConstantTimeSelect(int v, int x, int y);
} // namespace subtle
