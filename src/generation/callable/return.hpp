#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <stdexcept>

#include "generation/builder/builder.hpp"
#include "generation/callable/callable.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"

class ReturnCallableBuilder : public Callable {
public:
  ReturnCallableBuilder() : Callable() {};

  void
  call(Builder& builder,
       const std::vector<std::unique_ptr<Parameter>>& parameters) override {

    if (parameters.size() != 1) {
      // TODO: Better error message
      throw std::runtime_error("Print takes only one value");
    }

    const Parameter& parameter = *parameters[0];

    if (parameter.getType() == ParamaterType::VALUE) {
      const ParameterValue& parameterValue =
          static_cast<const ParameterValue&>(parameter);

      // TODO: Better error message
      if (!StringConverter::isInt(parameterValue.getValue())) {
        throw std::runtime_error("Cannot return non-int");
      }
      uint32_t value =
          StringConverter::toUnsignedLongLong(parameterValue.getValue());

      llvm::Value* extended =
          builder.zext(builder.createConst32(value), builder.getUint32());
      builder.createReturn(extended);
      return;
    }

    else if (parameter.getType() == ParamaterType::VARIABLE) {
      const ParameterVariable& parameterVariable =
          static_cast<const ParameterVariable&>(parameter);

      llvm::Value* extended = builder.zext(
          parameterVariable.getVariable().load(builder), builder.getUint32());
      builder.createReturn(extended);
    }
  }
};