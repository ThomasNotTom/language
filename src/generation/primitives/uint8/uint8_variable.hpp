#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"

class Uint8Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint8Variable(Builder& builder, const BuilderType& builderType)
      : Variable(builderType), llvmType(builder.getUint8()) {
    this->storage = builder.allocate(this->llvmType, "uint8");
  };

  llvm::LoadInst* load(Builder& builder) const {
    return builder.load(this->llvmType, this->storage, "uint8");
  }

  llvm::StoreInst* store(Builder& builder, uint8_t other) const {
    return builder.store(builder.createConst8(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder, Uint8Variable& other) const {
    return builder.store(other.load(builder), this->storage);
  };

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint8Variable& uint8Other =
          static_cast<const Uint8Variable&>(other);

      builder.add(this->storage, uint8Other.load(builder), "uint8_add_uint8");
    } else {
      throw std::runtime_error("No addition method is definedb between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    };
  };
};