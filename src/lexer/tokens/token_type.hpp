#pragma once

#include <cstdint>

enum TokenType : uint8_t {
  END_OF_LINE = 1,
  OPERATOR = 2,
  OTHER = 3,
  BRACKET_OPEN = 4,
  BRACKET_CLOSE = 5
};
