
#include "../syntax_analyser/program/program.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/statement.hpp"
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

    for (size_t i = 0; i < this->program.size(); i++) {
      const Statement& statement = program.get(i);

      switch (statement.statementType) {
        case StatementType::INITIALISATION: {
          InitialisationStatement* initialisationStatement =
              (InitialisationStatement*)&statement;

          switch (initialisationStatement->type) {
            case StatementPrimitiveType::UINT8: {
              llvm::Type* i8Type = llvm::Type::getInt8Ty(context);
              std::string identifierName =
                  initialisationStatement->identifier->name;

              llvm::AllocaInst* alloc = builder.CreateAlloca(
                  i8Type, nullptr, initialisationStatement->identifier->name);

              values[identifierName] = Variable{i8Type, alloc};
              break;
            }
          }
          break;
        }

        case StatementType::ASSIGNMENT: {
          AssignmentStatement* assignmentStatement =
              (AssignmentStatement*)&statement;

          std::string identifierName = assignmentStatement->identifier.name;
          if (!values.contains(assignmentStatement->identifier.name)) {
            std::string err = "Variable " +
                              assignmentStatement->identifier.name +
                              " does not exist\n";
            throw std::runtime_error(err);
          }

          Variable variable = values[identifierName];

          llvm::Value* value = llvm::ConstantInt::get(
              variable.type, assignmentStatement->value.value);
          builder.CreateStore(value, variable.alloc);
        }
      }
    }

    llvm::Value* returnValue =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);

    builder.CreateRet(returnValue);

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