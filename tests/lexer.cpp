#include "../src/lexer/lexer.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/token_type.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Complex get with no arguments to constructor", "[complex]") {
  Lexer lexer = Lexer("a;");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 2);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::IDENTIFIER);
  REQUIRE(tokenContainer.view(1).tokenType == TokenType::END_OF_LINE);
};