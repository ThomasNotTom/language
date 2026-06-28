#include "../syntax_analyser/program/program.hpp"
#include "generation/builder/builder.hpp"
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
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
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
#include <memory>
#include <optional>

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

    bool hasMainReturn = false;

    for (size_t i = 0; i < this->program.size(); i++) {
      const Statement& statement = program.view(i);

      switch (statement.statementType) {
        case StatementType::INITIALISATION: {
          const InitialisationStatement& initialisationStatement =
              static_cast<const InitialisationStatement&>(statement);

          switch (initialisationStatement.type) {
            case StatementPrimitiveType::UINT8: {
              std::string identifierName =
                  initialisationStatement.identifier->name;

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

              break;
            }

            case AssignmentType::IDENTIFIER: {
              const AssignmentIdentifierStatement&
                  assignmentIdentifierStatement =
                      static_cast<const AssignmentIdentifierStatement&>(
                          assignmentStatement);

              break;
            }
          }

          break;
        }

        case StatementType::RETURN: {
          const ReturnStatement& returnStatement =
              static_cast<const ReturnStatement&>(statement);

          break;
        }

        case StatementType::ADDITION: {
          const AdditionStatement& additionStatement =
              static_cast<const AdditionStatement&>(statement);

          switch (additionStatement.lhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {

              const IdentifierValue& lhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *additionStatement.lhs.get());

              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& lhsNumberValue =
                  static_cast<const NumberValue&>(*additionStatement.lhs.get());

              break;
            }
          }

          switch (additionStatement.rhs->statementValueType) {
            case StatementValueType::IDENTIFIER: {

              const IdentifierValue& rhsIdentifierValue =
                  static_cast<const IdentifierValue&>(
                      *additionStatement.rhs.get());

              break;
            }

            case StatementValueType::NUMBER: {
              const NumberValue& rhsNumberValue =
                  static_cast<const NumberValue&>(*additionStatement.rhs.get());

              break;
            }
          }
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