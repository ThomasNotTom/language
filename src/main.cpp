#include <iostream>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <string>
#include <utility>

#include "./io/file_reader.hpp"
#include "./lexer/lexer.hpp"
#include "./lexer/token_container/token_container.hpp"
#include "generation/generator.hpp"
#include "syntax_analyser/abstract_syntax_tree.hpp"
#include "syntax_analyser/program/program.hpp"

int main(const int argc, char* argv[]) {
  if (argc == 1) {
    std::cerr << "Provide an input file\n";
    return 1;
  }

  bool verbose = false;

  if (argc == 3) {
    if (std::string(argv[2]) == "-v") {
      verbose = true;
    }
  }

  FileReader fileReader(argv[1]);
  if (!fileReader.isOpen()) {
    std::cerr << "Input file failed to open\n";
    return 1;
  }
  ProgramText programText = fileReader.toProgramText();

  Lexer lexer(fileReader.readAll());
  TokenContainer tokens = lexer.makeTokenList();
  if (verbose) {
    tokens.print();
  }

  AbstractSyntaxTree ast(tokens, programText);

  Program program = ast.parse();
  if (verbose) {
    program.print();
  }

  Generator generator(program, programText);
  generator.init();

  llvm::LLVMContext context;
  std::unique_ptr<llvm::Module> module = generator.buildModule(context);

  if (verbose) {
    generator.print_module(*module);
  }

  generator.compile(context, std::move(module));
}