#pragma once

#include "generation/builder/builder.hpp"
// #include "generation/type.hpp"

class BuilderType;

class Variable {
protected:
  llvm::Value* storage;
  const BuilderType& builderType;

public:
  Variable(const BuilderType& builderType) : builderType(builderType) {};

  unsigned int getType() const;
  virtual void add(Builder& builder, const Variable& other) const = 0;
  virtual void returnValue(Builder& builder) const = 0;

  virtual ~Variable() = default;
};