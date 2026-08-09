#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"

class Uint16Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint16Variable(Builder& builder, const BuilderType& builderType)
      : Variable(builderType), llvmType(builder.getUint16()) {
    this->storage = builder.allocate(this->llvmType, "uint16");
  };

  llvm::LoadInst* load(Builder& builder) const {
    return builder.load(this->llvmType, this->storage, "uint16");
  }

  llvm::StoreInst* store(Builder& builder, int other) const override {
    return builder.store(builder.createConst16(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder,
                         const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint16Variable& uint16Other =
          static_cast<const Uint16Variable&>(other);

      return builder.store(uint16Other.load(builder), this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint16Variable& uint16Other =
          static_cast<const Uint16Variable&>(other);

      llvm::Value* addOut = builder.add(
          this->load(builder), uint16Other.load(builder), "uint16_add_uint16");

      builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is definedb between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  void add(Builder& builder, int other) const override {
    llvm::Value* addOut = builder.add(
        this->load(builder), builder.createConst16(other), "uint16_add_val");
    builder.store(addOut, this->storage);
  };

  void subtract(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint16Variable& uint16Other =
          static_cast<const Uint16Variable&>(other);

      llvm::Value* subOut = builder.subtract(
          this->load(builder), uint16Other.load(builder), "uint16_sub_uint16");

      builder.store(subOut, this->storage);
    } else {
      throw std::runtime_error(
          "No subtraction method is definedb between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  void subtract(Builder& builder, int other) const override {
    llvm::Value* subOut = builder.subtract(
        this->load(builder), builder.createConst16(other), "uint16_sub_val");

    builder.store(subOut, this->storage);
  };

  void print(Builder& builder,
             PrintCallableBuilder& printCallableBuilder) const override {
    printCallableBuilder.printLine(builder, this->load(builder));
  };

  void returnValue(Builder& builder) const override {
    llvm::Value* extended =
        builder.zext(this->load(builder), builder.getUint32());
    builder.createReturn(extended);
  };
};