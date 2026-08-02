#pragma once

#include "generation/builder/builder.hpp"
#include "generation/primitives/primitive.hpp"
#include "generation/primitives/uint_type.hpp"
#include "llvm/IR/DerivedTypes.h"

class BuilderUintPrimitive : public BuilderPrimitive {
protected:
  BuilderUintType uintType;
  llvm::IntegerType* llvmIntegerType;
  llvm::AllocaInst* alloc;

public:
  BuilderUintPrimitive(BuilderUintType uintType, Builder& builder,
                       llvm::IntegerType* llvmIntegerType, std::string name)
      : BuilderPrimitive(BuilderPrimitiveType::UINT, builder),
        llvmIntegerType(llvmIntegerType), uintType(uintType),
        alloc(builder.allocate(llvmIntegerType, name)) {};

  void assignValue(BuilderUintPrimitive& other) {
    builder.store(builder.load(this->llvmIntegerType, other.alloc, "load"),
                  this->alloc);
  };

  llvm::IntegerType* getLlvmIntegerType() { return this->llvmIntegerType; };
  llvm::AllocaInst* getAlloc() { return this->alloc; };

  BuilderUintType getUintType() { return this->uintType; }
};