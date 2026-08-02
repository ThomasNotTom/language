#include "../src/lexer/lexer.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/token_type.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Empty program", "[lexer]") {
  Lexer lexer = Lexer("");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 0);
};

TEST_CASE("Empty line", "[lexer]") {
  Lexer lexer = Lexer(";");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 1);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Singular variable declaration", "[lexer]") {
  Lexer lexer = Lexer("uint8 a;");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 3);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::PRIMITIVE);
  REQUIRE(tokenContainer.view(1).tokenType == TokenType::IDENTIFIER);
  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
};