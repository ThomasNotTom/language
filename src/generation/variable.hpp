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

  llvm::Value* getStorage() const { return this->storage; };
  const BuilderType& getBuilderType() const { return this->builderType; };

  virtual llvm::StoreInst* store(Builder& builder,
                                 const Variable& other) const = 0;

  virtual llvm::StoreInst* store(Builder& builder, std::string value) const = 0;

  virtual llvm::Value* add(Builder& builder, const Variable& other) const = 0;
  virtual llvm::Value* add(Builder& builder,
                           const std::string& other) const = 0;

  virtual llvm::Value* subtract(Builder& builder,
                                const Variable& other) const = 0;
  virtual llvm::Value* subtract(Builder& builder,
                                const std::string& other) const = 0;

  virtual llvm::Value* subtractFrom(Builder& builder,
                                    const Variable& other) const = 0;
  virtual llvm::Value* subtractFrom(Builder& builder,
                                    const std::string& other) const = 0;

  const InitialisationStatement& getInit() const {
    return this->initialisationStatement;
  };

  virtual llvm::Value* stringToLLVMValue(const Builder& builder,
                                         const std::string& value) const {
    return nullptr;
  }

  virtual ~Variable() = default;
};