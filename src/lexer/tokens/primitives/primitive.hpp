#pragma once

#include "../token.hpp"
#include "./primitive_type.hpp"

class Primitive : public Token {
public:
  const PrimitiveType primitiveType;
  Primitive(const PrimitiveType type);
};