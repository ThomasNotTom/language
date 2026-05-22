#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>

#include "./io/file_reader.hpp"
#include "./lexer/lexer.hpp"
#include "./lexer/token_container/token_container.hpp"
#include "syntax_analyser/abstract_syntax_tree.hpp"

int main(const int argc, char* argv[]) {
  if (argc == 1) {
    std::cerr << "Provide an input file\n";
    return 1;
  }

  FileReader fileReader(argv[1]);
  if (!fileReader.isOpen()) {
    std::cerr << "Input file failed to open\n";
    return 1;
  }

  Lexer lexer(fileReader.readAll());
  TokenContainer tokens = lexer.makeTokenList();
  tokens.print();

  AbstractSyntaxTree ast(tokens);

  Program program = ast.parse();
  program.print();
}