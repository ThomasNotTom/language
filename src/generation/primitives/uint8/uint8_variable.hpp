#pragma once

#include <stdexcept>

#include "generation/builder/builder.hpp"
#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"
#include "llvm/IR/Value.h"
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

    llvm::Value* value = stringToLLVMValue(builder, other);

    return builder.store(value, this->storage);
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

  llvm::Value* add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint8Variable& uint8Other =
          static_cast<const Uint8Variable&>(other);

      return builder.add(this->load(builder), uint8Other.load(builder),
                         "uint8_add_uint8");

      // builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  llvm::Value* add(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot add uint8 and non-uint8");
    }

    llvm::Value* value = stringToLLVMValue(builder, other);

    return builder.add(this->load(builder), value, "uint8_add_val");
    // builder.store(addOut, this->storage);
  };

  llvm::Value* subtract(Builder& builder,
                        const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint8Variable& uint8Other =
          static_cast<const Uint8Variable&>(other);

      return builder.subtract(this->load(builder), uint8Other.load(builder),
                              "uint8_sub_uint8");

    } else {
      throw std::runtime_error(
          "No subtraction method is defined between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  llvm::Value* subtract(Builder& builder,
                        const std::string& other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot subtract uint8 and non-uint8");
    }

    llvm::Value* value = stringToLLVMValue(builder, other);

    return builder.subtract(this->load(builder), value, "uint8_sub_val");
    // builder.store(subout, this->storage);
  };

  llvm::Value* subtractFrom(Builder& builder,
                            const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint8Variable& uint8Other =
          static_cast<const Uint8Variable&>(other);

      return builder.subtract(uint8Other.load(builder), this->load(builder),
                              "uint8_sub_uint8");

    } else {
      throw std::runtime_error(
          "No subtraction method is defined between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  llvm::Value* subtractFrom(Builder& builder,
                            const std::string& other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot subtract non-uint8 and uint8");
    }

    llvm::Value* value = stringToLLVMValue(builder, other);

    return builder.subtract(value, this->load(builder), "val_sub_uint8");
  };

  llvm::Value* stringToLLVMValue(const Builder& builder,
                                 const std::string& value) const override {
    return builder.createConst8(StringConverter::toUint8(value));
  };
};