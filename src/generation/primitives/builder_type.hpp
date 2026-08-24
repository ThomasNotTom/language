#pragma once

#include <cstdint>

enum class BuilderTypeID : uint8_t {
  UINT8 = 1,
  UINT16 = 2,
  UINT32 = 3,
  UINT64 = 4,

  FLOAT16 = 5,
  FLOAT32 = 6,
  FLOAT64 = 7
};