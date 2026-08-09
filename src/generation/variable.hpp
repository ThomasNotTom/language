#pragma once

#include "generation/builder/builder.hpp"
#include "generation/callable/print.hpp"

class BuilderType;

class Variable {
protected:
  llvm::Value* storage;
  const BuilderType& builderType;

public:
  Variable(const BuilderType& builderType) : builderType(builderType) {};

  unsigned int getType() const;
  virtual llvm::StoreInst* store(Builder& builder,
                                 const Variable& other) const = 0;

  virtual llvm::StoreInst* store(Builder& builder, int other) const = 0;

  virtual void add(Builder& builder, const Variable& other) const = 0;
  virtual void add(Builder& builder, int other) const = 0;

  virtual void subtract(Builder& builder, const Variable& other) const = 0;
  virtual void subtract(Builder& builder, int other) const = 0;

  virtual void print(Builder& builder,
                     PrintCallableBuilder& printCallableBuilder) const = 0;
  virtual void returnValue(Builder& builder) const = 0;

  virtual ~Variable() = default;
};