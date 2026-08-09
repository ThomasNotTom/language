#pragma once

#include <vector>

#include "generation/builder/builder.hpp"
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

class PrintCallableBuilder {
private:
  llvm::Function* printFunc;

public:
  PrintCallableBuilder(llvm::Module& module, Builder& builder) {
    auto* charPtrType = builder.getUint8Ptr();

    std::vector<llvm::Type*> PrintfArgsTypes = {charPtrType};

    llvm::FunctionType* PrintfType =
        llvm::FunctionType::get(builder.getUint32(), PrintfArgsTypes, true);

    this->printFunc = llvm::Function::Create(
        PrintfType, llvm::Function::ExternalLinkage, "printf", module);
  };

  void printLine(Builder& builder, llvm::Value* out) {
    llvm::Value* FormatStr = builder.createGlobalStringPtr("%llu\n");

    std::vector<llvm::Value*> Args = {FormatStr, out};

    builder.createCall(this->printFunc, Args);
  }
};