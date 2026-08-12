#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <vector>

#include "generation/builder/builder.hpp"
#include "generation/callable/callable.hpp"
#include "generation/variable.hpp"

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

  void call(Builder& builder, const Variable& out) override {
    llvm::Value* FormatStr = builder.createGlobalStringPtr("%llu\n");

    std::vector<llvm::Value*> Args = {FormatStr, out.load(builder)};

    builder.createCall(this->printFunc, Args);
  };

  void call(Builder& builder, uint64_t out) override {
    llvm::Value* FormatStr = builder.createGlobalStringPtr("%llu\n");

    std::vector<llvm::Value*> Args = {FormatStr, builder.createConst64(out)};

    builder.createCall(this->printFunc, Args);
  }
};