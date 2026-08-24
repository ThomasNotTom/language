#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Uint64Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint64Variable(Builder& builder, const BuilderType& builderType,
                 const InitialisationStatement& initialisationStatement)
      : Variable(builderType, initialisationStatement),
        llvmType(builder.getUint64()) {
    this->storage = builder.allocate(this->llvmType, "uint64");
  };

  llvm::LoadInst* load(Builder& builder) const override {
    return builder.load(this->llvmType, this->storage, "uint64");
  }

  llvm::StoreInst* store(Builder& builder, uint64_t other) const override {
    return builder.store(builder.createConst64(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder,
                         const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint64Variable& uint64Other =
          static_cast<const Uint64Variable&>(other);

      return builder.store(uint64Other.load(builder), this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint64Variable& uint64Other =
          static_cast<const Uint64Variable&>(other);

      llvm::Value* addOut = builder.add(
          this->load(builder), uint64Other.load(builder), "uint64_add_uint64");

      builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  void add(Builder& builder, uint64_t other) const override {
    llvm::Value* addOut = builder.add(
        this->load(builder), builder.createConst64(other), "uint64_add_val");
    builder.store(addOut, this->storage);
  };

  void subtract(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint64Variable& uint64Other =
          static_cast<const Uint64Variable&>(other);

      llvm::Value* subOut = builder.subtract(
          this->load(builder), uint64Other.load(builder), "uint64_sub_uint64");

      builder.store(subOut, this->storage);
    } else {
      throw std::runtime_error(
          "No subtraction method is defined between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  void subtract(Builder& builder, uint64_t other) const override {
    llvm::Value* subOut = builder.subtract(
        this->load(builder), builder.createConst64(other), "uint64_sub_val");

    builder.store(subOut, this->storage);
  };
};