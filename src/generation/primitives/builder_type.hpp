#pragma once

#include <cstdint>

enum class BuilderTypeID : uint8_t {
  UINT1 = 1,
  UINT8 = 2,
  UINT16 = 3,
  UINT32 = 4,
  UINT64 = 5,

  FLOAT16 = 6,
  FLOAT32 = 7,
  FLOAT64 = 8
};