#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"

class Uint8Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint8Variable(Builder& builder) : llvmType(builder.getUint8()) {
    this->storage = builder.allocate(this->llvmType, "uint8");
  };

  llvm::LoadInst* load(Builder& builder) {
    return builder.load(this->llvmType, this->storage, "uint8");
  }

  llvm::StoreInst* store(Builder& builder, uint8_t other) {
    return builder.store(builder.createConst8(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder, Uint8Variable& other) {
    return builder.store(other.load(builder), this->storage);
  }
};