#pragma once

#include <cstdint>
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <stdexcept>
#include <vector>

#include "generation/builder/builder.hpp"
#include "generation/callable/callable.hpp"
#include "generation/variable.hpp"
#include "lexer/string_converter.hpp"

class PrintCallableBuilder : public Callable {
private:
  llvm::Function* printFunc;

public:
  PrintCallableBuilder(llvm::Module& module, Builder& builder) : Callable() {
    auto* charPtrType = builder.getUint8Ptr();

    std::vector<llvm::Type*> PrintfArgsTypes = {charPtrType};

    llvm::FunctionType* PrintfType =
        llvm::FunctionType::get(builder.getUint32(), PrintfArgsTypes, true);

    this->printFunc = llvm::Function::Create(
        PrintfType, llvm::Function::ExternalLinkage, "printf", module);
  };

  void
  call(Builder& builder,
       const std::vector<std::unique_ptr<Parameter>>& parameters) override {

    if (parameters.size() != 1) {
      // TODO: Better error message
      throw std::runtime_error("Print takes only one value");
    }

    const Parameter& parameter = *parameters[0];
    std::vector<llvm::Value*> Args;

    std::cout << (uint16_t)parameter.getType() << "\n";
    if (parameter.getType() == ParamaterType::VALUE) {
      const ParameterValue& parameterValue =
          static_cast<const ParameterValue&>(parameter);
      std::cout << "Parsing: \"" << parameterValue.getValue() << "\"\n";

      if (StringConverter::isInt(parameterValue.getValue())) {
        std::cout << "adding int\n";

        llvm::Value* FormatStr = builder.createGlobalStringPtr("%llu\n");
        uint64_t value =
            StringConverter::toUnsignedLongLong(parameterValue.getValue());

        Args = {FormatStr, builder.createConst64(value)};
      }

      else if (StringConverter::isFloat(parameterValue.getValue())) {
        std::cout << "adding float\n";
        llvm::Value* FormatStr = builder.createGlobalStringPtr("%f\n");

        float value = StringConverter::toFloat(parameterValue.getValue());

        Args = {FormatStr, builder.createFloat16(value)};
      }

      else {
        throw std::runtime_error(
            "Parameter cannot be converted to either float or int");
      }
    }

    else if (parameter.getType() == ParamaterType::VARIABLE) {
      const ParameterVariable& parameterVariable =
          static_cast<const ParameterVariable&>(parameter);

      // TODO: Can only print integer variables (not floats)
      llvm::Value* FormatStr = builder.createGlobalStringPtr("%f\n");

      Args = {FormatStr, parameterVariable.getVariable().load(builder)};
    }
    builder.createCall(this->printFunc, Args);
  }
};
;