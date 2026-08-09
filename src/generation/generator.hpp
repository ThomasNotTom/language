#include "../syntax_analyser/program/program.hpp"
#include "generation/builder/builder.hpp"
#include "generation/primitives/uint16/uint16.hpp"
#include "generation/primitives/uint16/uint16_variable.hpp"
#include "generation/primitives/uint32/uint32.hpp"
#include "generation/primitives/uint32/uint32_variable.hpp"
#include "generation/primitives/uint64/uint64.hpp"
#include "generation/primitives/uint64/uint64_variable.hpp"
#include "generation/primitives/uint8/uint8.hpp"
#include "generation/primitives/uint8/uint8_variable.hpp"
#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "lexer/matcher.hpp"
#include "lexer/string_converter.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/print/print.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/subtraction/subtraction.hpp"
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

  std::unique_ptr<llvm::Module> buildModule(llvm::LLVMContext& context) {

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

    std::map<std::string, std::unique_ptr<BuilderType>> types =
        std::map<std::string, std::unique_ptr<BuilderType>>();

    // Create primitive types
    types.emplace("uint8", std::make_unique<Uint8Builder>(types.size()));
    types.emplace("uint16", std::make_unique<Uint16Builder>(types.size()));
    types.emplace("uint32", std::make_unique<Uint32Builder>(types.size()));
    types.emplace("uint64", std::make_unique<Uint64Builder>(types.size()));

    std::map<std::string, std::unique_ptr<Variable>> symbols =
        std::map<std::string, std::unique_ptr<Variable>>();

    std::map<std::string, std::unique_ptr<PrintCallableBuilder>> callables =
        std::map<std::string, std::unique_ptr<PrintCallableBuilder>>();

    callables.emplace("print",
                      std::make_unique<PrintCallableBuilder>(*module, builder));

    bool hasMainReturn = false;

    for (size_t i = 0; i < this->program.size(); i++) {
      const Statement& statement = program.view(i);

      switch (statement.statementType) {
        case StatementType::INITIALISATION: {
          const InitialisationStatement& initialisationStatement =
              static_cast<const InitialisationStatement&>(statement);

          const BuilderType& builderType =
              *types[initialisationStatement.type.name];

          symbols.emplace(initialisationStatement.identifier.name,
                          builderType.makeVariable(builder));

          break;
        }

        case StatementType::ASSIGNMENT: {
          const AssignmentStatement& assignmentStatement =
              static_cast<const AssignmentStatement&>(statement);

          const Variable& identifier =
              *symbols[assignmentStatement.identifier.name];

          if (Matcher::isInt(assignmentStatement.value.name)) {
            int valueInt = StringConverter::toUnsignedLongLong(
                assignmentStatement.value.name);

                identifier.store(builder, valueInt);
            break;
          }

          const Variable& value = *symbols[assignmentStatement.value.name];
          identifier.store(builder, value);

          break;
        }

        case StatementType::RETURN: {
          const ReturnStatement& returnStatement =
              static_cast<const ReturnStatement&>(statement);

          const Variable& value = *symbols[returnStatement.value.name];

          value.returnValue(builder);
          hasMainReturn = true;

          break;
        }

        case StatementType::ADDITION: {
          const AdditionStatement& additionStatement =
              static_cast<const AdditionStatement&>(statement);

          const Variable& lhs = *symbols[additionStatement.lhs.name];

          if (Matcher::isInt(additionStatement.rhs.name)) {
            int valueInt =
                StringConverter::toUnsignedLongLong(additionStatement.rhs.name);
            lhs.add(builder, valueInt);
            break;
          }

          const Variable& rhs = *symbols[additionStatement.rhs.name];
          lhs.add(builder, rhs);
          break;
        }

        case StatementType::SUBTRACTION: {
          const SubtractionStatement& subtractionStatement =
              static_cast<const SubtractionStatement&>(statement);

          const Variable& lhs = *symbols[subtractionStatement.lhs.name];

          if (Matcher::isInt(subtractionStatement.rhs.name)) {
            int valueInt = StringConverter::toUnsignedLongLong(
                subtractionStatement.rhs.name);
            lhs.subtract(builder, valueInt);
            break;
          }
          const Variable& rhs = *symbols[subtractionStatement.rhs.name];
          lhs.subtract(builder, rhs);
          break;
        }

        case StatementType::PRINT: {
          const PrintStatement& printStatement =
              static_cast<const PrintStatement&>(statement);

          const Variable& value = *symbols[printStatement.value.name];
          value.print(builder, *callables["print"]);
        }
      }
    }
    if (!hasMainReturn) {
      llvm::ConstantInt* returnValue = builder.createConst32(0);
      builder.createReturn(returnValue);
    }

    return module;
  }

  void print_module(const llvm::Module& module) {
    std::cout << "-- LLVM IR --" << std::endl;
    module.print(llvm::outs(), nullptr);
  }

  void compile(llvm::LLVMContext& context,
               std::unique_ptr<llvm::Module> module) {

    auto targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    module->setTargetTriple(targetTriple);

    std::string error;

    const llvm::Target* target =
        llvm::TargetRegistry::lookupTarget(targetTriple, error);

    auto CPU = "generic";
    auto features = "";
    llvm::TargetOptions opt;
    std::optional<llvm::Reloc::Model> RM = llvm::Reloc::PIC_;

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