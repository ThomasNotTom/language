#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include "generation/builder/builder.hpp"
#include "generation/callable/callable.hpp"
#include "generation/variable.hpp"

class ReturnCallableBuilder : public Callable {
public:
  ReturnCallableBuilder() : Callable() {};

  void call(Builder& builder, const Variable& out) override {
    llvm::Value* extended =
        builder.zext(out.load(builder), builder.getUint32());
    builder.createReturn(extended);
  }

  void call(Builder& builder, uint64_t out) override {
    llvm::Value* extended =
        builder.zext(builder.createConst64(out), builder.getUint32());
    builder.createReturn(extended);
  }
};