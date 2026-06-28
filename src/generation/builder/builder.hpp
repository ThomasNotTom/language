#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
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

  llvm::ConstantInt* createConst8(uint8_t value) {
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), value);
  };

  llvm::ConstantInt* createConst16(uint16_t value) {
    return llvm::ConstantInt::get(llvm::Type::getInt16Ty(context), value);
  };

  llvm::ConstantInt* createConst32(uint32_t value) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), value);
  };

  llvm::ReturnInst* createReturn(llvm::Value* value) {
    return this->irBuilder.CreateRet(value);
  };
};