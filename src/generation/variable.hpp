#pragma once

#include "syntax_analyser/statement/initialisation/initialisation.hpp"
class BuilderType;
class Builder;

#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Value.h>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Variable {
protected:
  llvm::Value* storage;
  const BuilderType& builderType;
  const InitialisationStatement& initialisationStatement;

public:
  Variable(const BuilderType& builderType,
           const InitialisationStatement& initialisationStatement)
      : builderType(builderType),
        initialisationStatement(initialisationStatement) {};

  unsigned int getType() const;

  virtual llvm::LoadInst* load(Builder& builder) const = 0;

  virtual llvm::StoreInst* store(Builder& builder,
                                 const Variable& other) const = 0;

  virtual llvm::StoreInst* store(Builder& builder, std::string value) const = 0;

  virtual void add(Builder& builder, const Variable& other) const = 0;
  virtual void add(Builder& builder, uint64_t other) const = 0;

  virtual void subtract(Builder& builder, const Variable& other) const = 0;
  virtual void subtract(Builder& builder, uint64_t other) const = 0;

  const InitialisationStatement& getInit() const {
    return this->initialisationStatement;
  };

  virtual ~Variable() = default;
};