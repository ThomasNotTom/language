#pragma once

#include "generation/builder/builder.hpp"
#include "generation/type.hpp"

class Variable {
protected:
  // const BuilderType& builderType;
  llvm::Value* storage;

public:
  Variable() {};
};