#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"

class Uint32Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint32Variable(Builder& builder, const BuilderType& builderType)
      : Variable(builderType), llvmType(builder.getUint32()) {
    this->storage = builder.allocate(this->llvmType, "uint32");
  };

  llvm::LoadInst* load(Builder& builder) const override {
    return builder.load(this->llvmType, this->storage, "uint32");
  }

  llvm::StoreInst* store(Builder& builder, uint64_t other) const override {
    return builder.store(builder.createConst32(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder,
                         const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint32Variable& uint32Other =
          static_cast<const Uint32Variable&>(other);

      return builder.store(uint32Other.load(builder), this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint32Variable& uint32Other =
          static_cast<const Uint32Variable&>(other);

      llvm::Value* addOut = builder.add(
          this->load(builder), uint32Other.load(builder), "uint32_add_uint32");

      builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is definedb between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  void add(Builder& builder, uint64_t other) const override {
    llvm::Value* addOut = builder.add(
        this->load(builder), builder.createConst32(other), "uint32_add_val");
    builder.store(addOut, this->storage);
  };

  void subtract(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint32Variable& uint32Other =
          static_cast<const Uint32Variable&>(other);

      llvm::Value* subOut = builder.subtract(
          this->load(builder), uint32Other.load(builder), "uint32_sub_uint32");

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
        this->load(builder), builder.createConst32(other), "uint32_sub_val");

    builder.store(subOut, this->storage);
  };
};