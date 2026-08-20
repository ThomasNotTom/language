#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Float16Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Float16Variable(Builder& builder, const BuilderType& builderType,
                  const InitialisationStatement& initialisationStatement)
      : Variable(builderType, initialisationStatement),
        llvmType(builder.getFloat16()) {
    this->storage = builder.allocate(this->llvmType, "float16");
  };

  llvm::LoadInst* load(Builder& builder) const override {
    return builder.load(this->llvmType, this->storage, "float16");
  }

  llvm::StoreInst* store(Builder& builder, uint64_t other) const override {
    return builder.store(builder.createConst16(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder,
                         const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float16Variable& Float16Other =
          static_cast<const Float16Variable&>(other);

      return builder.store(Float16Other.load(builder), this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float16Variable& float16Other =
          static_cast<const Float16Variable&>(other);

      llvm::Value* addOut =
          builder.add(this->load(builder), float16Other.load(builder),
                      "float16_add_float16");

      builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is definedb between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  void add(Builder& builder, uint64_t other) const override {
    llvm::Value* addOut = builder.add(
        this->load(builder), builder.createConst16(other), "float16_add_val");
    builder.store(addOut, this->storage);
  };

  void subtract(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float16Variable& Float16Other =
          static_cast<const Float16Variable&>(other);

      llvm::Value* subOut =
          builder.subtract(this->load(builder), Float16Other.load(builder),
                           "float16_sub_float16");

      builder.store(subOut, this->storage);
    } else {
      throw std::runtime_error(
          "No subtraction method is definedb between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  void subtract(Builder& builder, uint64_t other) const override {
    llvm::Value* subOut = builder.subtract(
        this->load(builder), builder.createConst16(other), "float16_sub_val");

    builder.store(subOut, this->storage);
  };
};