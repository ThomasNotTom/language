#pragma once

#include <stdexcept>

#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Float64Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Float64Variable(Builder& builder, const BuilderType& builderType,
                  const InitialisationStatement& initialisationStatement)
      : Variable(builderType, initialisationStatement),
        llvmType(builder.getFloat64()) {
    this->storage = builder.allocate(this->llvmType, "float64");
  };

  llvm::LoadInst* load(Builder& builder) const override {
    return builder.load(this->llvmType, this->storage, "float64");
  }

  llvm::StoreInst* store(Builder& builder, std::string other) const override {
    if (!StringConverter::isDouble(other)) {
      // TODO: Improve error message
      throw std::runtime_error("Cannot store non-float to float");
    }
    double value = StringConverter::toDouble(other);
    return builder.store(builder.createFloat64(value), this->storage);
  }

  llvm::StoreInst* store(Builder& builder,
                         const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float64Variable& float64Other =
          static_cast<const Float64Variable&>(other);

      return builder.store(float64Other.load(builder), this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float64Variable& float64Other =
          static_cast<const Float64Variable&>(other);

      llvm::Value* addOut =
          builder.addf(this->load(builder), float64Other.load(builder),
                       "float64_add_float64");

      builder.store(addOut, this->storage);
    } else {
      throw std::runtime_error("No addition method is defined between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  };

  void add(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isDouble(other)) {
      throw std::runtime_error("Cannot add double and non-double");
    }

    double value = StringConverter::toDouble(other);
    llvm::Value* addOut = builder.addf(
        this->load(builder), builder.createFloat64(value), "float64_add_val");

    builder.store(addOut, this->storage);
  };

  void subtract(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Float64Variable& float64Other =
          static_cast<const Float64Variable&>(other);

      llvm::Value* subOut =
          builder.subtractf(this->load(builder), float64Other.load(builder),
                            "float64_sub_float64");

      builder.store(subOut, this->storage);
    } else {
      throw std::runtime_error(
          "No subtraction method is defined between type " +
          std::to_string(THIS_TYPE) + " and " +
          std::to_string(other.getType()));
    };
  };

  void subtract(Builder& builder, const std::string& other) const override {
    if (!StringConverter::isDouble(other)) {
      throw std::runtime_error("Cannot subtract double and non-double");
    }

    double value = StringConverter::toDouble(other);
    llvm::Value* subOut = builder.subtractf(
        this->load(builder), builder.createFloat64(value), "float64_sub_val");

    builder.store(subOut, this->storage);
  };
};