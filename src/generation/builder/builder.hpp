#pragma once

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <cstdint>

class Builder {
private:
  llvm::IRBuilder<> irBuilder;
  llvm::LLVMContext& context;

public:
  Builder(llvm::LLVMContext& context) : irBuilder(context), context(context) {}

  void setInsertPoint(llvm::BasicBlock* i) {
    this->irBuilder.SetInsertPoint(i);
  }

  llvm::IntegerType* getUint8() const { return llvm::Type::getInt8Ty(context); }

  llvm::ConstantInt* createConst8(uint8_t value) {
    return llvm::ConstantInt::get(this->getUint8(), value);
  };

  llvm::IntegerType* getUint16() const {
    return llvm::Type::getInt16Ty(context);
  }

  llvm::ConstantInt* createConst16(uint16_t value) {
    return llvm::ConstantInt::get(this->getUint16(), value);
  };

  llvm::IntegerType* getUint32() const {
    return llvm::Type::getInt32Ty(context);
  }

  llvm::ConstantInt* createConst32(uint32_t value) {
    return llvm::ConstantInt::get(this->getUint32(), value);
  };

  llvm::ReturnInst* createReturn(llvm::Value* value) {
    return this->irBuilder.CreateRet(value);
  };

  llvm::AllocaInst* allocate(llvm::Type* type, std::string name) {
    return this->irBuilder.CreateAlloca(type, nullptr, name);
  };

  llvm::StoreInst* store(llvm::Value* value, llvm::Value* out) {
    return this->irBuilder.CreateStore(value, out);
  };
};