#pragma once

#include "generation/builder/builder.hpp"
#include "llvm/IR/Instructions.h"
#include <cstdint>

class BuilderUint8 {
private:
  Builder& builder;

  llvm::AllocaInst* alloc;

public:
  BuilderUint8(Builder& builder, std::string name)
      : builder(builder),
        alloc(this->builder.allocate(builder.getUint8(), name)) {};

  void storeValue(uint8_t value) {
    this->builder.store(builder.createConst8(value), this->alloc);
  };

  void assignValue(Builder& builder, BuilderUint8& other) {
    builder.store(builder.load(builder.getUint8(), other.alloc), this->alloc);
  };

  llvm::AllocaInst* getAlloc() { return this->alloc; }
};