#pragma once

#include "generation/builder/builder.hpp"
#include "generation/primitives/primitive_type.hpp"

class BuilderPrimitive {
protected:
  BuilderPrimitiveType type;

  Builder& builder;

public:
  BuilderPrimitive(BuilderPrimitiveType type, Builder& builder)
      : type(type), builder(builder) {};

  BuilderPrimitiveType getType() { return this->type; }
};