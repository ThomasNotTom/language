#pragma once

#include <cstdint>

#include "../primitive.hpp"

class Integer : Primitive {
private:
  const uint32_t value;

public:
  Integer(const uint32_t value);
};