#include "../syntax_analyser/program/program.hpp"
#include "generation/builder/builder.hpp"
#include "generation/primitives/primitive.hpp"
#include "generation/primitives/uint16.hpp"
#include "generation/primitives/uint32.hpp"
#include "generation/primitives/uint64.hpp"
#include "generation/primitives/uint8.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/assignment/assignment_type.hpp"
#include "syntax_analyser/statement/assignment/identifier/identifier.hpp"
#include "syntax_analyser/statement/assignment/number/number.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/print/print.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/subtraction/subtraction.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/number/number.hpp"
#include "syntax_analyser/statement/value/value.hpp"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Program.h"
#include <iostream>
#include <llvm/CodeGen/TargetPassConfig.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>

class Generator {
private:
  const Program& program;

public:
  Generator(const Program& program) : program(program) {}

  void init() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
  }

  void compile() {

    llvm::LLVMContext context;

    std::unique_ptr<llvm::Module> module =
        std::make_unique<llvm::Module>("build", context);

    Builder builder = Builder(context);

    llvm::FunctionType* mainFuncType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(context), {}, false);

    llvm::Function* mainFunc = llvm::Function::Create(
        mainFuncType, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock* mainEntry =
        llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.setInsertPoint(mainEntry);

    // Initialise print
    auto* charPtrType = builder.getUint8Ptr();

    std::vector<llvm::Type*> PrintfArgsTypes = {charPtrType};

    llvm::FunctionType* PrintfType =
        llvm::FunctionType::get(builder.getUint32(), PrintfArgsTypes, true);

    llvm::Function* PrintfFunc = llvm::Function::Create(
        PrintfType, llvm::Function::ExternalLinkage, "printf", *module);

    // End print init

    std::map<std::string, std::unique_ptr<BuilderPrimitive>> symbols;

    bool hasMainReturn = false;

    for (size_t i = 0; i < this->program.size(); i++) {
      const Statement& statement = program.view(i);

      switch (statement.statementType) {
        case StatementType::INITIALISATION: {
          const InitialisationStatement& initialisationStatement =
              static_cast<const InitialisationStatement&>(statement);

          std::string identifierName = initialisationStatement.identifier->name;

          switch (initialisationStatement.type) {
            case StatementPrimitiveType::UINT8: {
              symbols.emplace(identifierName, std::make_unique<BuilderUint8>(
                                                  builder, identifierName));
              break;
            }

            case StatementPrimitiveType::UINT16: {
              symbols.emplace(identifierName, std::make_unique<BuilderUint16>(
                                                  builder, identifierName));
              break;
            }

            case StatementPrimitiveType::UINT32: {
              symbols.emplace(identifierName, std::make_unique<BuilderUint32>(
                                                  builder, identifierName));
              break;
            }

            case StatementPrimitiveType::UINT64: {
              symbols.emplace(identifierName, std::make_unique<BuilderUint64>(
                                                  builder, identifierName));
              break;
            }
          }
          break;
        }

        case StatementType::ASSIGNMENT: {
          const AssignmentStatement& assignmentStatement =
              static_cast<const AssignmentStatement&>(statement);

          switch (assignmentStatement.assignmentType) {
            case AssignmentType::NUMBER: {
              const AssignmentNumberStatement& assignmentNumberStatement =
                  static_cast<const AssignmentNumberStatement&>(
                      assignmentStatement);

              std::unique_ptr<BuilderPrimitive>& prim =
                  symbols.at(assignmentNumberStatement.identifier.name);

              switch (prim->getType()) {
                case BuilderPrimitiveType::UINT: {
                  BuilderUintPrimitive& uintPrim =
                      static_cast<BuilderUintPrimitive&>(*prim);

                  switch (uintPrim.getUintType()) {
                    case BuilderUintType::UINT8: {
                      BuilderUint8& uint8Prim =
                          static_cast<BuilderUint8&>(uintPrim);

                      uint8Prim.storeValue(
                          assignmentNumberStatement.value.value);
                      break;
                    }

                    case BuilderUintType::UINT16: {
                      BuilderUint16& uint16Prim =
                          static_cast<BuilderUint16&>(uintPrim);

                      uint16Prim.storeValue(
                          assignmentNumberStatement.value.value);
                      break;
                    }

                    case BuilderUintType::UINT32: {
                      BuilderUint32& uint32Prim =
                          static_cast<BuilderUint32&>(uintPrim);

                      uint32Prim.storeValue(
                          assignmentNumberStatement.value.value);
                      break;
                    }

                    case BuilderUintType::UINT64: {
                      BuilderUint64& uint64Prim =
                          static_cast<BuilderUint64&>(uintPrim);

                      uint64Prim.storeValue(
                          assignmentNumberStatement.value.value);
                      break;
                    }
                  }
                }
              }
              break;
            }

            case AssignmentType::IDENTIFIER: {
              const AssignmentIdentifierStatement&
                  assignmentIdentifierStatement =
                      static_cast<const AssignmentIdentifierStatement&>(
                          assignmentStatement);

              std::unique_ptr<BuilderPrimitive>& inBase =
                  symbols.at(assignmentIdentifierStatement.value.name);

              if (inBase->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot add non-uint type");
              }

              BuilderUintPrimitive& in =
                  static_cast<BuilderUintPrimitive&>(*inBase);

              std::unique_ptr<BuilderPrimitive>& outBase =
                  symbols.at(assignmentIdentifierStatement.identifier.name);

              if (outBase->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot add non-uint type");
              }

              BuilderUintPrimitive& out =
                  static_cast<BuilderUintPrimitive&>(*outBase);

              out.assignValue(in);

              break;
            }
          }

          break;
        }

        case StatementType::RETURN: {
          const ReturnStatement& returnStatement =
              static_cast<const ReturnStatement&>(statement);

          llvm::Value* returnValue;

          switch (returnStatement.value->statementValueType) {
            case StatementValueType::NUMBER: {
              const NumberValue& numberValue =
                  static_cast<const NumberValue&>(*returnStatement.value);

              returnValue = builder.createConst32(numberValue.value);
              break;
            }

            case StatementValueType::IDENTIFIER: {
              const IdentifierValue& identifierValue =
                  static_cast<const IdentifierValue&>(*returnStatement.value);

              std::unique_ptr<BuilderPrimitive>& returnBuilder =
                  symbols.at(identifierValue.name);

              if (returnBuilder->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot return non-uint type");
              }

              BuilderUintPrimitive& returnBuilderUint =
                  static_cast<BuilderUintPrimitive&>(*returnBuilder);

              llvm::Value* rawOut = builder.load(
                  returnBuilderUint.getLlvmIntegerType(),
                  returnBuilderUint.getAlloc(), identifierValue.name + "_load");

              returnValue = builder.zext(rawOut, builder.getUint32());
            }
          }

          builder.createReturn(returnValue);

          hasMainReturn = true;

          break;
        }

        case StatementType::ADDITION: {
          const AdditionStatement& additionStatement =
              static_cast<const AdditionStatement&>(statement);

          std::unique_ptr<BuilderPrimitive>& out =
              symbols.at(additionStatement.identifier.name);

          if (out->getType() != BuilderPrimitiveType::UINT) {
            throw std::runtime_error("Cannot assign addition to non-uint type");
          }

          BuilderUintPrimitive& outUint =
              static_cast<BuilderUintPrimitive&>(*out);

          llvm::Value* lhs;
          llvm::Value* rhs;

          std::string outName = "add_";

          switch (additionStatement.lhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {
              const IdentifierValue& lhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *additionStatement.lhs.get());

              std::unique_ptr<BuilderPrimitive>& lhsValue =
                  symbols.at(lhsIdentifierValue.name);

              if (lhsValue->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot add non-uint types");
              }

              BuilderUintPrimitive& lhsUintValue =
                  static_cast<BuilderUintPrimitive&>(*lhsValue);

              lhs = builder.load(lhsUintValue.getLlvmIntegerType(),
                                 lhsUintValue.getAlloc(),
                                 lhsIdentifierValue.name + "_load");

              outName += lhsIdentifierValue.name;
              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& lhsNumberValue =
                  static_cast<const NumberValue&>(*additionStatement.lhs.get());

              lhs = builder.createConst8(lhsNumberValue.value);
              outName += "const";
              break;
            }
          }

          outName += "_and_";

          switch (additionStatement.rhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {
              const IdentifierValue& rhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *additionStatement.rhs.get());

              std::unique_ptr<BuilderPrimitive>& rhsValue =
                  symbols.at(rhsIdentifierValue.name);

              if (rhsValue->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot add non-uint types");
              }

              BuilderUintPrimitive& rhsValueUint =
                  static_cast<BuilderUintPrimitive&>(*rhsValue);

              rhs = builder.load(rhsValueUint.getLlvmIntegerType(),
                                 rhsValueUint.getAlloc(),
                                 rhsIdentifierValue.name + "_load");

              outName += rhsIdentifierValue.name;
              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& rhsNumberValue =
                  static_cast<const NumberValue&>(*additionStatement.rhs.get());

              rhs = builder.createConst8(rhsNumberValue.value);
              outName += "const";

              break;
            }
          }

          builder.store(builder.add(lhs, rhs, outName), outUint.getAlloc());
        }

        case StatementType::SUBTRACTION: {
          const SubtractionStatement& subtractionStatement =
              static_cast<const SubtractionStatement&>(statement);

          std::unique_ptr<BuilderPrimitive>& out =
              symbols.at(subtractionStatement.identifier.name);

          if (out->getType() != BuilderPrimitiveType::UINT) {
            throw std::runtime_error(
                "Cannot assign subtraction to non-uint type");
          }

          BuilderUintPrimitive& outUint =
              static_cast<BuilderUintPrimitive&>(*out);

          llvm::Value* lhs;
          llvm::Value* rhs;

          std::string outName = "subtract_";

          switch (subtractionStatement.lhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {
              const IdentifierValue& lhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *subtractionStatement.lhs.get());

              std::unique_ptr<BuilderPrimitive>& lhsValue =
                  symbols.at(lhsIdentifierValue.name);

              if (lhsValue->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot subtract non-uint types");
              }

              BuilderUintPrimitive& lhsUintValue =
                  static_cast<BuilderUintPrimitive&>(*lhsValue);

              lhs = builder.load(lhsUintValue.getLlvmIntegerType(),
                                 lhsUintValue.getAlloc(),
                                 lhsIdentifierValue.name + "_load");

              outName += lhsIdentifierValue.name;
              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& lhsNumberValue =
                  static_cast<const NumberValue&>(
                      *subtractionStatement.lhs.get());

              lhs = builder.createConst8(lhsNumberValue.value);
              outName += "const";
              break;
            }
          }

          outName += "_and_";

          switch (subtractionStatement.rhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {
              const IdentifierValue& rhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *subtractionStatement.rhs.get());

              std::unique_ptr<BuilderPrimitive>& rhsValue =
                  symbols.at(rhsIdentifierValue.name);

              if (rhsValue->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot subtract non-uint types");
              }

              BuilderUintPrimitive& rhsValueUint =
                  static_cast<BuilderUintPrimitive&>(*rhsValue);

              rhs = builder.load(rhsValueUint.getLlvmIntegerType(),
                                 rhsValueUint.getAlloc(),
                                 rhsIdentifierValue.name + "_load");

              outName += rhsIdentifierValue.name;
              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& rhsNumberValue =
                  static_cast<const NumberValue&>(
                      *subtractionStatement.rhs.get());

              rhs = builder.createConst8(rhsNumberValue.value);
              outName += "const";

              break;
            }
          }

          builder.store(builder.subtract(lhs, rhs, outName),
                        outUint.getAlloc());
          break;
        }

        case StatementType::PRINT: {
          const PrintStatement& printStatement =
              static_cast<const PrintStatement&>(statement);

          llvm::Value* out;

          switch (printStatement.value->statementValueType) {
            case StatementValueType::NUMBER: {
              const NumberValue& numberValue =
                  static_cast<const NumberValue&>(*printStatement.value);

              out = builder.createConst32(numberValue.value);
              break;
            }

            case StatementValueType::IDENTIFIER: {
              const IdentifierValue& identifierValue =
                  static_cast<const IdentifierValue&>(*printStatement.value);

              std::unique_ptr<BuilderPrimitive>& printBuilder =
                  symbols.at(identifierValue.name);

              if (printBuilder->getType() != BuilderPrimitiveType::UINT) {
                throw std::runtime_error("Cannot print non-uint type");
              }

              BuilderUintPrimitive& printBuilderUint =
                  static_cast<BuilderUintPrimitive&>(*printBuilder);

              out = builder.load(printBuilderUint.getLlvmIntegerType(),
                                 printBuilderUint.getAlloc(), "temp");
            }
          }

          llvm::Value* FormatStr = builder.createGlobalStringPtr("%llu\n");

          std::vector<llvm::Value*> Args = {FormatStr, out};

          builder.createCall(PrintfFunc, Args);
        }
      }
    }
    if (!hasMainReturn) {
      llvm::ConstantInt* returnValue = builder.createConst32(0);
      builder.createReturn(returnValue);
    }

    std::cout << "-- LLVM IR --" << std::endl;
    module->print(llvm::outs(), nullptr);

    auto targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    module->setTargetTriple(targetTriple);

    std::string error;

    const llvm::Target* target =
        llvm::TargetRegistry::lookupTarget(targetTriple, error);

    auto CPU = "generic";
    auto features = "";
    llvm::TargetOptions opt;
    std::optional<llvm::Reloc::Model> RM = std::nullopt;

    llvm::TargetMachine* targetMachine =
        target->createTargetMachine(targetTriple, CPU, features, opt, RM);

    module->setDataLayout(targetMachine->createDataLayout());

    auto filename = "output.o";
    std::error_code ec;
    llvm::raw_fd_ostream dest(filename, ec, llvm::sys::fs::OF_None);

    llvm::legacy::PassManager pass;

    auto fileType = llvm::CodeGenFileType::ObjectFile;

    targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType);

    pass.run(*module);
    dest.flush();

    llvm::ErrorOr<std::string> clangPath =
        llvm::sys::findProgramByName("clang");

    std::vector<llvm::StringRef> args = {clangPath.get(), "output.o", "-o",
                                         "main.out"};

    int linkResult = llvm::sys::ExecuteAndWait(clangPath.get(), args,
                                               std::nullopt, {}, 0, 0, &error);
  }
};