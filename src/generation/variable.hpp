#pragma once

class BuilderType;
class Builder;

#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Value.h>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

class Variable {
protected:
  llvm::Value* storage;
  const BuilderType& builderType;

public:
  Variable(const BuilderType& builderType) : builderType(builderType) {};

  unsigned int getType() const;

  virtual llvm::LoadInst* load(Builder& builder) const = 0;

  virtual llvm::StoreInst* store(Builder& builder,
                                 const Variable& other) const = 0;

  virtual llvm::StoreInst* store(Builder& builder, uint64_t other) const = 0;

  virtual void add(Builder& builder, const Variable& other) const = 0;
  virtual void add(Builder& builder, uint64_t other) const = 0;

  virtual void subtract(Builder& builder, const Variable& other) const = 0;
  virtual void subtract(Builder& builder, uint64_t other) const = 0;

  virtual ~Variable() = default;
};