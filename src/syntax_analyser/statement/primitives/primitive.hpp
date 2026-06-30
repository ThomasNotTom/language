#pragma once

#include "./primitive_type.hpp"

class StatementPrimitive {
public:
  const StatementPrimitiveType primitiveType;
  StatementPrimitive(const StatementPrimitiveType type);
};