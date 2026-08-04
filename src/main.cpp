#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>

#include "./io/file_reader.hpp"
#include "./lexer/lexer.hpp"
#include "./lexer/token_container/token_container.hpp"
#include "generation/generator.hpp"
#include "syntax_analyser/abstract_syntax_tree.hpp"

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

  Lexer lexer(fileReader.readAll());
  TokenContainer tokens = lexer.makeTokenList();
  if (verbose) {
    tokens.print();
  }

  AbstractSyntaxTree ast(tokens);

  Program program = ast.parse();
  if (verbose) {
    program.print();
  }

  Generator generator(program);
  generator.init();

  llvm::LLVMContext context;
  std::unique_ptr<llvm::Module> module = generator.buildModule(context);

  if (verbose) {
    generator.print_module(std::move(module));
  }

  generator.compile(context, std::move(module));
}