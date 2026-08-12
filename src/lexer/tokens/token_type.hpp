#pragma once

#include <cstdint>

enum TokenType : uint8_t {
  END_OF_LINE = 1,
  OPERATOR = 2,
  RETURN = 3,
  PRINT = 4,
  OTHER = 5
};
