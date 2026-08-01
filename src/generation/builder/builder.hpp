#pragma once

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
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
  llvm::PointerType* getUint8Ptr() const {
    return llvm::PointerType::get(this->context, 0);
  }

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

  llvm::IntegerType* getUint64() const {
    return llvm::Type::getInt64Ty(context);
  }

  llvm::ConstantInt* createConst64(uint64_t value) {
    return llvm::ConstantInt::get(this->getUint64(), value);
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

  llvm::LoadInst* load(llvm::Type* type, llvm::Value* value, std::string name) {
    return this->irBuilder.CreateLoad(type, value, name);
  };

  llvm::Value* add(llvm::Value* lhs, llvm::Value* rhs, std::string name) {
    return this->irBuilder.CreateAdd(lhs, rhs, name);
  };

  llvm::Value* zext(llvm::Value* in, llvm::Type* outType) {
    return this->irBuilder.CreateZExt(in, outType);
  };

  llvm::Value* createGlobalStringPtr(std::string str) {
    return this->irBuilder.CreateGlobalString(str);
  };

  llvm::CallInst* createCall(llvm::Function* function,
                             std::vector<llvm::Value*> args) {
    return this->irBuilder.CreateCall(function, args);
  }
};