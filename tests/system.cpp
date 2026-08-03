#include <catch2/catch_test_macros.hpp>

#include "generation/generator.hpp"
#include "io/file_reader.hpp"
#include "lexer/lexer.hpp"
#include "lexer/token_container/token_container.hpp"
#include "syntax_analyser/abstract_syntax_tree.hpp"

TEST_CASE("Run \"./examples/addition.lang\"", "[system]") {
  FileReader fileReader("./examples/addition.lang");
  Lexer lexer = Lexer(fileReader.readAll());

  TokenContainer tokens = lexer.makeTokenList();
  tokens.print();

  AbstractSyntaxTree ast(tokens);

  Program program = ast.parse();
  program.print();

  Generator generator(program);
  generator.init();
  generator.compile();
};
