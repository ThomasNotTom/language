#pragma once

#include <stdexcept>

#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Float32Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Float32Variable(Builder& builder, const BuilderType& builderType,
                  const InitialisationStatement& initialisationStatement)
      : Variable(builderType, initialisationStatement),
        llvmType(builder.getFloat32()) {
    this->storage = builder.allocate(this->llvmType, "float32");
  };

  llvm::LoadInst* load(Builder& builder) const override {
    return builder.load(this->llvmType, this->storage, "float32");
  }

  llvm::StoreInst* store(Builder& builder, std::string other) const override {
    if (!StringConverter::isDouble(other)) {
      // TODO: Improve error message
      throw std::runtime_error("Cannot store non-float to float");
    }
    float value = static_cast<float>(StringConverter::toDouble(other));
    return builder.store(builder.createFloat32(value), this->storage);
  }

  llvm::StoreInst* store(Builder& builder,
                         const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float32Variable& float32Other =
          static_cast<const Float32Variable&>(other);

      return builder.store(float32Other.load(builder), this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  llvm::Value* add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float32Variable& float32Other =
          static_cast<const Float32Variable&>(other);

      return builder.addf(this->load(builder), float32Other.load(builder),
                          "float32_add_float32");

      // builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  llvm::Value* add(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isDouble(other)) {
      throw std::runtime_error("Cannot add float and non-float");
    }

    float value = static_cast<float>(StringConverter::toDouble(other));
    return builder.addf(this->load(builder), builder.createFloat32(value),
                        "float32_add_val");

    // builder.store(addOut, this->storage);
  };

  llvm::Value* subtract(Builder& builder,
                        const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float32Variable& float32Other =
          static_cast<const Float32Variable&>(other);

      return builder.subtractf(this->load(builder), float32Other.load(builder),
                               "float32_sub_float32");

      // builder.store(subOut, this->storage);
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
      throw std::runtime_error("Cannot subtract float and non-float");
    }

    float value = static_cast<float>(StringConverter::toDouble(other));
    return builder.subtractf(this->load(builder), builder.createFloat32(value),
                             "float32_sub_val");

    // builder.store(subOut, this->storage);
  };

  llvm::Value* subtractFrom(Builder& builder,
                            const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float32Variable& Float32Other =
          static_cast<const Float32Variable&>(other);

      return builder.subtractf(Float32Other.load(builder), this->load(builder),
                               "float32_sub_float32");

      // builder.store(subOut, this->storage);
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
    return builder.subtractf(builder.createFloat32(value), this->load(builder),
                             "val_sub_float32");

    // builder.store(subOut, this->storage);
  };
};