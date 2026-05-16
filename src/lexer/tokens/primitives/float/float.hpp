#pragma once

#include "../primitive.hpp"

class Float : Primitive {
private:
  const float value;

public:
  Float(const float value);
};