#pragma once
#include "unsafe.h"
namespace race {
void Acquire(void *addr);
void Disable();
void Enable();
constexpr bool Enabled{false};
int Errors();
void Read(void *addr);
void ReadRange(void *addr, int len);
void Release(void *addr);
void ReleaseMerge(void *addr);
void Write(void *addr);
void WriteRange(void *addr, int len);
} // namespace race
