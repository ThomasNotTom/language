#pragma once

class BuilderType;
class Builder;

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Value.h>

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

  virtual llvm::StoreInst* store(Builder& builder, int other) const = 0;

  virtual void add(Builder& builder, const Variable& other) const = 0;
  virtual void add(Builder& builder, int other) const = 0;

  virtual void subtract(Builder& builder, const Variable& other) const = 0;
  virtual void subtract(Builder& builder, int other) const = 0;

  virtual ~Variable() = default;
};