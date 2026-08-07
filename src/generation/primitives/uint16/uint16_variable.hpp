#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"

class Uint16Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint16Variable(Builder& builder) : llvmType(builder.getUint16()) {
    this->storage = builder.allocate(this->llvmType, "uint16");
  };

  llvm::LoadInst* load(Builder& builder) {
    return builder.load(this->llvmType, this->storage, "uint16");
  }

  llvm::StoreInst* store(Builder& builder, uint16_t other) {
    return builder.store(builder.createConst16(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder, Uint16Variable& other) {
    return builder.store(other.load(builder), this->storage);
  }
};