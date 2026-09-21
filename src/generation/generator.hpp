#include <cstddef>
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
#include <string>
#include <vector>

#include "../syntax_analyser/program/program.hpp"
#include "generation/builder/builder.hpp"
#include "generation/callable/callable.hpp"
#include "generation/callable/print.hpp"
#include "generation/callable/return.hpp"
#include "generation/context_container.hpp"
#include "generation/primitives/float16/float16.hpp"
#include "generation/primitives/float32/float32.hpp"
#include "generation/primitives/float64/float64.hpp"
#include "generation/primitives/uint1/uint1.hpp"
#include "generation/primitives/uint16/uint16.hpp"
#include "generation/primitives/uint32/uint32.hpp"
#include "generation/primitives/uint64/uint64.hpp"
#include "generation/primitives/uint8/uint8.hpp"
#include "generation/type.hpp"
#include "generation/variable.hpp"
#include "io/program_text.hpp"
#include "lexer/tokens/other.hpp"
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

    ContextContainer contextContainer = ContextContainer();
    contextContainer.makeEmptyContext();

    contextContainer.addType("uint1", std::make_unique<Uint1Builder>());
    contextContainer.addType("uint8", std::make_unique<Uint8Builder>());
    contextContainer.addType("uint16", std::make_unique<Uint16Builder>());
    contextContainer.addType("uint32", std::make_unique<Uint32Builder>());
    contextContainer.addType("uint64", std::make_unique<Uint64Builder>());

    contextContainer.addType("float16", std::make_unique<Float16Builder>());
    contextContainer.addType("float32", std::make_unique<Float32Builder>());
    contextContainer.addType("float64", std::make_unique<Float64Builder>());

    contextContainer.addCallable(
        "print", std::make_unique<PrintCallableBuilder>(*module, builder));

    contextContainer.addCallable("return",
                                 std::make_unique<ReturnCallableBuilder>());

    bool hasMainReturn = false;

    for (size_t i = 0; i < this->program.size(); i++) {
      const Statement& statement = program.view(i);

      std::map<std::string, BuilderType*> currentTypes =
          contextContainer.getTypes();

      std::map<std::string, Variable*> currentSymbols =
          contextContainer.getSymbols();

      std::map<std::string, Callable*> currentCallables =
          contextContainer.getCallables();

      switch (statement.statementType) {
        case StatementType::INITIALISATION: {
          const InitialisationStatement& initialisationStatement =
              static_cast<const InitialisationStatement&>(statement);

          const BuilderType& builderType =
              *currentTypes.at(initialisationStatement.type.name);

          if (currentSymbols.contains(
                  initialisationStatement.identifier.name)) {
            Variable& previousDeclaration =
                (*currentSymbols.at(initialisationStatement.identifier.name));

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

          contextContainer.addSymbol(
              initialisationStatement.identifier.name,
              builderType.makeVariable(builder, initialisationStatement));

          break;
        }

        case StatementType::ASSIGNMENT: {
          const AssignmentStatement& assignmentStatement =
              static_cast<const AssignmentStatement&>(statement);

          if (!currentSymbols.contains(assignmentStatement.identifier.name)) {
            const std::string& assignmentStatementLineNumber = std::to_string(
                assignmentStatement.identifier.metadata.line + 1);

            const std::string& assignmentStatementLine =
                this->programText.getLine(
                    assignmentStatement.identifier.metadata.line);

            std::string out = "\n";
            out += assignmentStatementLineNumber;
            out += ": ";
            out += assignmentStatementLine;
            out += "\nVariable \"";
            out += assignmentStatement.identifier.name;
            out += "\" has not been initialised";

            throw std::runtime_error(out);
          }
          const Variable& identifier =
              *currentSymbols.at(assignmentStatement.identifier.name);

          if (!currentSymbols.contains(assignmentStatement.value.name)) {
            identifier.store(builder, assignmentStatement.value.name);
            break;
          }

          const Variable& value =
              *currentSymbols.at(assignmentStatement.value.name);
          identifier.store(builder, value);

          break;
        }

        case StatementType::ADDITION: {
          const AdditionStatement& additionStatement =
              static_cast<const AdditionStatement&>(statement);

          const Variable& identifier =
              *currentSymbols.at(additionStatement.identifier.name);

          const bool lhsExists =
              currentSymbols.contains(additionStatement.lhs.name);
          const bool rhsExists =
              currentSymbols.contains(additionStatement.rhs.name);

          // TODO: Fix dubtraction of primitive - primitive

          if (!lhsExists && !rhsExists) {
            llvm::Value* lhsValue = identifier.getBuilderType().makeValue(
                builder, additionStatement.lhs.name);
            if (lhsValue == nullptr) {
              throw std::runtime_error(
                  "No conversion between \"" + additionStatement.lhs.name +
                  "\" to type " + std::to_string(identifier.getType()));
            }
            llvm::Value* rhsValue = identifier.getBuilderType().makeValue(
                builder, additionStatement.rhs.name);
            if (rhsValue == nullptr) {
              throw std::runtime_error(
                  "No conversion between \"" + additionStatement.rhs.name +
                  "\" to type " + std::to_string(identifier.getType()));
            }
            llvm::Value* temp = builder.add(lhsValue, rhsValue, "temp");

            builder.store(temp, identifier.getStorage());
            break;
          }
          // TODO: Fix addition of floats and primitive + variable

          if (lhsExists && rhsExists) {
            const Variable& lhs =
                *currentSymbols.at(additionStatement.lhs.name);
            const Variable& rhs =
                *currentSymbols.at(additionStatement.rhs.name);

            builder.store(lhs.add(builder, rhs), identifier.getStorage());
            break;
          }

          if (lhsExists) {
            const Variable& lhs =
                *currentSymbols.at(additionStatement.lhs.name);

            builder.store(lhs.add(builder, additionStatement.rhs.name),
                          identifier.getStorage());

            break;
          }

          if (rhsExists) {
            const Variable& rhs =
                *currentSymbols.at(additionStatement.rhs.name);
            builder.store(rhs.add(builder, additionStatement.lhs.name),
                          identifier.getStorage());
            break;
          }

          break;
        }

        case StatementType::SUBTRACTION: {
          const SubtractionStatement& subtractionStatement =
              static_cast<const SubtractionStatement&>(statement);
          const Variable& identifier =
              *currentSymbols.at(subtractionStatement.identifier.name);

          const bool lhsExists =
              currentSymbols.contains(subtractionStatement.lhs.name);
          const bool rhsExists =
              currentSymbols.contains(subtractionStatement.rhs.name);

          // TODO: Fix dubtraction of primitive - primitive

          if (!lhsExists && !rhsExists) {
            llvm::Value* lhsValue = identifier.getBuilderType().makeValue(
                builder, subtractionStatement.lhs.name);
            if (lhsValue == nullptr) {
              throw std::runtime_error(
                  "No conversion between \"" + subtractionStatement.lhs.name +
                  "\" to type " + std::to_string(identifier.getType()));
            }
            llvm::Value* rhsValue = identifier.getBuilderType().makeValue(
                builder, subtractionStatement.rhs.name);
            if (rhsValue == nullptr) {
              throw std::runtime_error(
                  "No conversion between \"" + subtractionStatement.rhs.name +
                  "\" to type " + std::to_string(identifier.getType()));
            }
            llvm::Value* temp = builder.subtract(lhsValue, rhsValue, "temp");

            builder.store(temp, identifier.getStorage());
            break;
          }
          // TODO: Fix addition of floats and primitive + variable

          if (lhsExists && rhsExists) {
            const Variable& lhs =
                *currentSymbols.at(subtractionStatement.lhs.name);
            const Variable& rhs =
                *currentSymbols.at(subtractionStatement.rhs.name);

            builder.store(lhs.subtract(builder, rhs), identifier.getStorage());
            break;
          }

          if (lhsExists) {
            const Variable& lhs =
                *currentSymbols.at(subtractionStatement.lhs.name);

            builder.store(lhs.subtract(builder, subtractionStatement.rhs.name),
                          identifier.getStorage());

            break;
          }

          if (rhsExists) {
            const Variable& rhs =
                *currentSymbols.at(subtractionStatement.rhs.name);
            builder.store(
                rhs.subtractFrom(builder, subtractionStatement.lhs.name),
                identifier.getStorage());
            break;
          }
          break;
        };

        case StatementType::FUNCTION_CALL: {
          const FunctionCallStatement& functionCallStatement =
              static_cast<const FunctionCallStatement&>(statement);
          const std::string& functionCallLine = this->programText.getLine(
              functionCallStatement.identifier.metadata.line);

          const std::string& functionCallLineNumber = std::to_string(
              functionCallStatement.identifier.metadata.line + 1);
          const std::string& functionName =
              functionCallStatement.identifier.name;
          if (!currentCallables.contains(functionName)) {
            std::string out = "\n";
            out += functionCallLineNumber;
            out += ": ";
            out += functionCallLine;
            out += "\nFunction \"";
            out += functionCallStatement.identifier.name;
            out += "\" is not defined";
            throw std::runtime_error(out);
          }

          std::vector<std::unique_ptr<Parameter>> parameters =
              std::vector<std::unique_ptr<Parameter>>();

          for (size_t i = 0; i < functionCallStatement.parameters.size(); i++) {
            const std::string& name = functionCallStatement.parameters[i].name;
            if (!currentSymbols.contains(name)) {
              parameters.push_back(std::make_unique<ParameterValue>(name));
              continue;
            }

            const Variable& variable = *currentSymbols.at(name);
            parameters.push_back(std::make_unique<ParameterVariable>(variable));
          }

          (*currentCallables.at(functionCallStatement.identifier.name))
              .call(builder, parameters);

          if (functionName == "return") {
            hasMainReturn = true;
          }
          continue;
        }
        case StatementType::CONTEXT_BEGIN: {
          contextContainer.makeEmptyContext();
          break;
        }

        case StatementType::CONTEXT_END: {
          contextContainer.removeTopContext();
          break;
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