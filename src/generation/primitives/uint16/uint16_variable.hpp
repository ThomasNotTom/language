#pragma once

#include "generation/type.hpp"
#include "generation/variable.hpp"

class Uint16Variable : public Variable {
private:
  llvm::Type* llvmType;

public:
  Uint16Variable(Builder& builder, const BuilderType& builderType)
      : Variable(builderType), llvmType(builder.getUint16()) {
    this->storage = builder.allocate(this->llvmType, "uint16");
  };

  llvm::LoadInst* load(Builder& builder) const {
    return builder.load(this->llvmType, this->storage, "uint16");
  }

  llvm::StoreInst* store(Builder& builder, uint16_t other) const {
    return builder.store(builder.createConst16(other), this->storage);
  }

  llvm::StoreInst* store(Builder& builder, Uint16Variable& other) const {
    return builder.store(other.load(builder), this->storage);
  }

  void add(Builder& builder, const Variable& other) const override {
    const unsigned int THIS_TYPE = this->getType();

    if (other.getType() == THIS_TYPE) {
      const Uint16Variable& uint16Other =
          static_cast<const Uint16Variable&>(other);

      builder.add(this->storage, uint16Other.load(builder),
                  "uint16_add_uint16");
    } else {
      throw std::runtime_error("No addition method is definedb between type " +
                               std::to_string(THIS_TYPE) + " and " +
                               std::to_string(other.getType()));
    }
  }
};