#pragma once

#include <stdexcept>

#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"
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

  llvm::StoreInst* store(Builder& builder, std::string other) const override {
    if (!StringConverter::isDouble(other)) {
      // TODO: Improve error message
      throw std::runtime_error("Cannot store non-float to float");
    }
    float value = static_cast<float>(StringConverter::toDouble(other));
    return builder.store(builder.createFloat16(value), this->storage);
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

  llvm::Value* add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float16Variable& float16Other =
          static_cast<const Float16Variable&>(other);

      return builder.addf(this->load(builder), float16Other.load(builder),
                          "float16_add_float16");

      // builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  llvm::Value* add(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isDouble(other)) {
      throw std::runtime_error("Cannot add half and non-half");
    }

    float value = static_cast<float>(StringConverter::toDouble(other));
    return builder.addf(this->load(builder), builder.createFloat16(value),
                        "float16_add_val");
  };

  llvm::Value* subtract(Builder& builder,
                        const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float16Variable& Float16Other =
          static_cast<const Float16Variable&>(other);

      return builder.subtractf(this->load(builder), Float16Other.load(builder),
                               "float16_sub_float16");

    } else {
      throw std::runtime_error(
          "No subtraction method is defined between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  llvm::Value* subtract(Builder& builder,
                        const std::string& other) const override {
    if (!StringConverter::isDouble(other)) {
      throw std::runtime_error("Cannot subtract half and non-half");
    }

    float value = static_cast<float>(StringConverter::toDouble(other));
    return builder.subtractf(this->load(builder), builder.createFloat16(value),
                             "float16_sub_val");
  };

  llvm::Value* subtractFrom(Builder& builder,
                            const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float16Variable& Float16Other =
          static_cast<const Float16Variable&>(other);

      return builder.subtractf(Float16Other.load(builder), this->load(builder),
                               "float16_sub_float16");

    } else {
      throw std::runtime_error(
          "No subtraction method is defined between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  llvm::Value* subtractFrom(Builder& builder,
                            const std::string& other) const override {
    if (!StringConverter::isDouble(other)) {
      throw std::runtime_error("Cannot subtract non-half and half");
    }

    float value = static_cast<float>(StringConverter::toDouble(other));
    return builder.subtractf(builder.createFloat16(value), this->load(builder),
                             "val_sub_float16");
  };
};