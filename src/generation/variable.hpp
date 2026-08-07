#pragma once

#include "generation/builder/builder.hpp"
#include "generation/type.hpp"

class Variable {
protected:
  llvm::Value* storage;

public:
  Variable() {};
};