#pragma once

#include <stdexcept>

#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Uint8Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint8Variable(Builder& builder, const BuilderType& builderType,
                const InitialisationStatement& initialisationStatement)
      : Variable(builderType, initialisationStatement),
        llvmType(builder.getUint8()) {
    this->storage = builder.allocate(this->llvmType, "uint8");
  };

  llvm::LoadInst* load(Builder& builder) const override {
    return builder.load(this->llvmType, this->storage, "uint8");
  }

  llvm::StoreInst* store(Builder& builder, std::string other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot convert non-int to int");
    }

    uint8_t value = StringConverter::toUnsignedLongLong(other);

    return builder.store(builder.createConst8(value), this->storage);
  }

  llvm::StoreInst* store(Builder& builder,
                         const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint8Variable& uint8Other =
          static_cast<const Uint8Variable&>(other);

      return builder.store(uint8Other.load(builder), this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint8Variable& uint8Other =
          static_cast<const Uint8Variable&>(other);

      llvm::Value* addOut = builder.add(
          this->load(builder), uint8Other.load(builder), "uint8_add_uint8");

      builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  void add(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot add uint8 and non-uint8");
    }

    uint8_t value =
        static_cast<uint8_t>(StringConverter::toUnsignedLongLong(other));

    llvm::Value* addOut = builder.add(
        this->load(builder), builder.createConst8(value), "uint8_add_val");
    builder.store(addOut, this->storage);
  };

  void subtract(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint8Variable& uint8Other =
          static_cast<const Uint8Variable&>(other);

      llvm::Value* subOut = builder.subtract(
          this->load(builder), uint8Other.load(builder), "uint8_sub_uint8");

      builder.store(subOut, this->storage);
    } else {
      throw std::runtime_error(
          "No subtraction method is defined between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  void subtract(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot subtract uint8 and non-uint8");
    }

    uint8_t value =
        static_cast<uint8_t>(StringConverter::toUnsignedLongLong(other));

    llvm::Value* subout = builder.subtract(
        this->load(builder), builder.createConst8(value), "uint8_sub_val");
    builder.store(subout, this->storage);
  };
};