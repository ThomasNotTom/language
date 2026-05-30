
#include "../syntax_analyser/program/program.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/assignment/assignment_type.hpp"
#include "syntax_analyser/statement/assignment/identifier/identifier.hpp"
#include "syntax_analyser/statement/assignment/number/number.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/number/number.hpp"
#include "syntax_analyser/statement/value/value.hpp"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Program.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <iostream>
#include <llvm/CodeGen/TargetPassConfig.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
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
    struct Variable {
      llvm::Type* type;
      llvm::AllocaInst* alloc;
      llvm::Value* value;
    };

    std::map<std::string, Variable> values;

    llvm::LLVMContext context;

    std::unique_ptr<llvm::Module> module =
        std::make_unique<llvm::Module>("build", context);

    llvm::IRBuilder<> builder(context);

    llvm::FunctionType* mainFuncType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(context), {}, false);

    llvm::Function* mainFunc = llvm::Function::Create(
        mainFuncType, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock* mainEntry =
        llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(mainEntry);

    bool hasMainReturn = false;

    for (size_t i = 0; i < this->program.size(); i++) {
      const Statement& statement = program.view(i);

      switch (statement.statementType) {
        case StatementType::INITIALISATION: {
          const InitialisationStatement& initialisationStatement =
              static_cast<const InitialisationStatement&>(statement);

          switch (initialisationStatement.type) {
            case StatementPrimitiveType::UINT8: {
              llvm::Type* i8Type = llvm::Type::getInt8Ty(context);
              std::string identifierName =
                  initialisationStatement.identifier->name;

              llvm::AllocaInst* alloc = builder.CreateAlloca(
                  i8Type, nullptr, initialisationStatement.identifier->name);

              values[identifierName] = Variable{i8Type, alloc, nullptr};
              break;
            }
          }
          break;
        }

        case StatementType::ASSIGNMENT: {
          const AssignmentStatement& assignmentStatement =
              static_cast<const AssignmentStatement&>(statement);

          if (!values.contains(assignmentStatement.identifier.name)) {
            std::string err = "Variable " +
                              assignmentStatement.identifier.name +
                              " does not exist\n";
            throw std::runtime_error(err);
          }

          std::string identifierName = assignmentStatement.identifier.name;
          Variable variable = values[identifierName];

          switch (assignmentStatement.assignmentType) {
            case AssignmentType::NUMBER: {
              const AssignmentNumberStatement& assignmentNumberStatement =
                  static_cast<const AssignmentNumberStatement&>(
                      assignmentStatement);

              llvm::Value* value = llvm::ConstantInt::get(
                  variable.type, assignmentNumberStatement.value.value);

              builder.CreateStore(value, variable.alloc);
              values[identifierName].value = value;
              break;
            }

            case AssignmentType::IDENTIFIER: {
              const AssignmentIdentifierStatement&
                  assignmentIdentifierStatement =
                      static_cast<const AssignmentIdentifierStatement&>(
                          assignmentStatement);

              Variable otherVariable =
                  values[assignmentIdentifierStatement.value.name];

              builder.CreateStore(otherVariable.value, variable.alloc);
              values[identifierName].value = otherVariable.value;

              break;
            }
          }

          break;
        }

        case StatementType::RETURN: {
          const ReturnStatement& returnStatement =
              static_cast<const ReturnStatement&>(statement);
          Variable returnValue = values[returnStatement.identifier->name];

          builder.CreateRet(returnValue.value);
          hasMainReturn = true;
          break;
        }

        case StatementType::ADDITION: {
          const AdditionStatement& additionStatement =
              static_cast<const AdditionStatement&>(statement);

          Variable variable = values[additionStatement.identifier.name];

          llvm::Value* lhs;
          switch (additionStatement.lhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {

              const IdentifierValue& lhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *additionStatement.lhs.get());

              lhs = values[lhsIdentifierValue.name].value;

              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& lhsNumberValue =
                  static_cast<const NumberValue&>(*additionStatement.lhs.get());
              lhs = llvm::ConstantInt::get(variable.type, lhsNumberValue.value);
              break;
            }
          }

          llvm::Value* rhs;
          switch (additionStatement.rhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {
              const IdentifierValue& lhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *additionStatement.rhs.get());

              Variable rhsVariable = values[lhsIdentifierValue.name];

              rhs = rhsVariable.value;
              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& rhsNumberValue =
                  static_cast<const NumberValue&>(*additionStatement.rhs.get());
              rhs = llvm::ConstantInt::get(variable.type, rhsNumberValue.value);
              break;
            }
          }

          llvm::Value* result = builder.CreateAdd(lhs, rhs);

          builder.CreateStore(result, variable.alloc);

          values[additionStatement.identifier.name].value = result;
        }
      }
    }
    if (!hasMainReturn) {
      llvm::Value* returnValue =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);

      builder.CreateRet(returnValue);
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