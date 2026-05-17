#pragma once

#include "../token.hpp"
#include "./primitive_type.hpp"

class PrimitiveToken : public Token {
public:
  const PrimitiveType primitiveType;
  PrimitiveToken(const PrimitiveType type);
};