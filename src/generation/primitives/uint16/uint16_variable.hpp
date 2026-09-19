#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"
#include "llvm/IR/Value.h"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Uint16Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint16Variable(Builder& builder, const BuilderType& builderType,
                 const InitialisationStatement& initialisationStatement)
      : Variable(builderType, initialisationStatement),
        llvmType(builder.getUint16()) {
    this->storage = builder.allocate(this->llvmType, "uint16");
  };

  llvm::LoadInst* load(Builder& builder) const override {
    return builder.load(this->llvmType, this->storage, "uint16");
  }

  llvm::StoreInst* store(Builder& builder, std::string other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot convert non-int to int");
    }

    llvm::Value* value = this->builderType.makeValue(builder, other);

    return builder.store(value, this->storage);
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

  llvm::Value* add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint16Variable& uint16Other =
          static_cast<const Uint16Variable&>(other);

      return builder.add(this->load(builder), uint16Other.load(builder),
                         "uint16_add_uint16");

      // builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  llvm::Value* add(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isInt(other)) {
      throw std::runtime_error("Cannot add uint16 and non-uint16");
    }

    llvm::Value* value = this->builderType.makeValue(builder, other);

    return builder.add(this->load(builder), value, "uint16_add_val");
    // builder.store(addOut, this->storage);
  };

  llvm::Value* subtract(Builder& builder,
                        const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint16Variable& uint16Other =
          static_cast<const Uint16Variable&>(other);

      return builder.subtract(this->load(builder), uint16Other.load(builder),
                              "uint16_sub_uint16");

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
      throw std::runtime_error("Cannot add uint16 and non-uint16");
    }

    llvm::Value* value = this->builderType.makeValue(builder, other);

    return builder.subtract(this->load(builder), value, "uint16_sub_val");
  };

  llvm::Value* subtractFrom(Builder& builder,
                            const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint16Variable& uint16Other =
          static_cast<const Uint16Variable&>(other);

      return builder.subtract(uint16Other.load(builder), this->load(builder),
                              "uint16_sub_uint16");

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
      throw std::runtime_error("Cannot subtract non-uint16 and uint16");
    }

    llvm::Value* value = this->builderType.makeValue(builder, other);

    return builder.subtract(value, this->load(builder), "val_sub_uint16");
  };
};