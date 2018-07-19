#pragma once
#include "binary.h"
namespace chacha20 {
void XORKeyStream(moku::slice<uint8_t> out,
                  moku::slice<uint8_t> in,
                  std::vector<uint8_t> *counter,
                  std::vector<uint8_t> *key);
void core(std::vector<uint8_t> *out,
          std::vector<uint8_t> *in,
          std::vector<uint8_t> *k);
static constexpr int rounds{20};
} // namespace chacha20
