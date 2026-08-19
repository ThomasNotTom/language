#include <cstddef>
#include <cstdint>
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

#include "../syntax_analyser/program/program.hpp"
#include "generation/builder/builder.hpp"
#include "generation/callable/print.hpp"
#include "generation/callable/return.hpp"
#include "generation/primitives/uint16/uint16.hpp"
#include "generation/primitives/uint32/uint32.hpp"
#include "generation/primitives/uint64/uint64.hpp"
#include "generation/primitives/uint8/uint8.hpp"
#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "io/program_text.hpp"
#include "lexer/matcher.hpp"
#include "lexer/string_converter.hpp"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Program.h"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/function_call/function_call.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/subtraction/subtraction.hpp"

class Generator {
private:
  const Program& program;
  const ProgramText programText;

public:
  Generator(const Program& program, const ProgramText& programText)
      : program(program), programText(programText) {}

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

    std::map<std::string, std::unique_ptr<Callable>> callables =
        std::map<std::string, std::unique_ptr<Callable>>();

    callables.emplace("print",
                      std::make_unique<PrintCallableBuilder>(*module, builder));

    callables.emplace("return", std::make_unique<ReturnCallableBuilder>());

    bool hasMainReturn = false;

    for (size_t i = 0; i < this->program.size(); i++) {
      const Statement& statement = program.view(i);
      switch (statement.statementType) {
        case StatementType::INITIALISATION: {
          const InitialisationStatement& initialisationStatement =
              static_cast<const InitialisationStatement&>(statement);

          const BuilderType& builderType =
              *types[initialisationStatement.type.name];

          if (symbols.contains(initialisationStatement.identifier.name)) {
            // std::cout << "Error: ";
            Variable& previousDeclaration =
                (*symbols[initialisationStatement.identifier.name]);
            // program.printInitialisationStatement(initialisationStatement);

            const InitialisationStatement& previousInitialisationStatement =
                previousDeclaration.getInit();

            const std::string& previousDeclarationLine =
                this->programText.getLine(
                    previousInitialisationStatement.type.metadata.line);

            const std::string& previousDeclarationLineNumber = std::to_string(
                previousInitialisationStatement.type.metadata.line + 1);

            const std::string& currentDeclarationLine =
                this->programText.getLine(
                    initialisationStatement.type.metadata.line);

            const std::string& currentDeclarationLineNumber =
                std::to_string(initialisationStatement.type.metadata.line + 1);

            std::string out = "\n";
            out += previousDeclarationLineNumber;
            out += ": ";
            out += previousDeclarationLine;
            out += "\n";
            out += currentDeclarationLineNumber;
            out += ": ";
            out += currentDeclarationLine;
            out += "\nVariable \"";
            out += initialisationStatement.identifier.name;
            out += "\" has already been initialised in line ";
            out += previousDeclarationLineNumber;
            throw std::runtime_error(out);
          }

          symbols.emplace(
              initialisationStatement.identifier.name,
              builderType.makeVariable(builder, initialisationStatement));

          break;
        }

        case StatementType::ASSIGNMENT: {
          const AssignmentStatement& assignmentStatement =
              static_cast<const AssignmentStatement&>(statement);

          if (!symbols.contains(assignmentStatement.identifier.name)) {
            std::cout << "Error: ";
            program.printAssignmentStatement(assignmentStatement);

            throw std::runtime_error("Assigning to uninitialised variable \"" +
                                     assignmentStatement.identifier.name +
                                     "\"");
          }
          const Variable& identifier =
              *symbols[assignmentStatement.identifier.name];

          if (Matcher::isInt(assignmentStatement.value.name)) {
            uint64_t valueInt = StringConverter::toUnsignedLongLong(
                assignmentStatement.value.name);

            identifier.store(builder, valueInt);
            break;
          }

          const Variable& value = *symbols[assignmentStatement.value.name];
          identifier.store(builder, value);

          break;
        }

        case StatementType::ADDITION: {
          const AdditionStatement& additionStatement =
              static_cast<const AdditionStatement&>(statement);

          const Variable& lhs = *symbols[additionStatement.lhs.name];

          if (Matcher::isInt(additionStatement.rhs.name)) {
            uint64_t valueInt =
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
            uint64_t valueInt = StringConverter::toUnsignedLongLong(
                subtractionStatement.rhs.name);
            lhs.subtract(builder, valueInt);
            break;
          }
          const Variable& rhs = *symbols[subtractionStatement.rhs.name];
          lhs.subtract(builder, rhs);
          break;
        }

        case StatementType::FUNCTION_CALL: {
          const FunctionCallStatement& functionCallStatement =
              static_cast<const FunctionCallStatement&>(statement);
          const Variable& value =
              *symbols[functionCallStatement.parameters[0].name];

          if (!callables.contains(functionCallStatement.identifier.name)) {
            throw std::runtime_error("Function \"" +
                                     functionCallStatement.identifier.name +
                                     "\" does not exist");
          }
          (*callables[functionCallStatement.identifier.name])
              .call(builder, value);
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
    std::cout << "-- LLVM IR --\n";
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