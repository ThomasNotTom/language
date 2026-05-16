#pragma once

#include "../token.hpp"
#include "./primitive_type.hpp"

class Primitive : Token {
private:
  PrimitiveType primitiveType;

public:
  Primitive(const PrimitiveType type);
};