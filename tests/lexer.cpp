#include "../src/lexer/lexer.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/identifier/identifier.hpp"
#include "lexer/tokens/primitives/primitive.hpp"
#include "lexer/tokens/primitives/primitive_type.hpp"
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

void test_primitive_variable_declaration(std::string string,
                                         PrimitiveType primitive) {
  std::string IDENTIFIER = "a";

  Lexer lexer = Lexer(string + " " + IDENTIFIER + ";");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 3);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::PRIMITIVE);
  REQUIRE(static_cast<const PrimitiveToken&>(tokenContainer.view(0))
              .primitiveType == primitive);

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::IDENTIFIER);
  REQUIRE(static_cast<const IdentifierToken&>(tokenContainer.view(1)).name ==
          IDENTIFIER);

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
}

TEST_CASE("Singular uint8 variable declaration", "[lexer]") {
  test_primitive_variable_declaration("uint8", PrimitiveType::UINT8);
};

TEST_CASE("Singular uint16 variable declaration", "[lexer]") {
  test_primitive_variable_declaration("uint16", PrimitiveType::UINT16);
};

TEST_CASE("Singular uint32 variable declaration", "[lexer]") {
  test_primitive_variable_declaration("uint32", PrimitiveType::UINT32);
};

TEST_CASE("Singular uint64 variable declaration", "[lexer]") {
  test_primitive_variable_declaration("uint64", PrimitiveType::UINT64);
};

TEST_CASE("Return number", "[lexer]") {
  Lexer lexer = Lexer("return 0;");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 3);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::RETURN);

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::NUMBER);
  REQUIRE(static_cast<const NumberToken&>(tokenContainer.view(1)).value == 0);

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
}

TEST_CASE("Return identifier", "[lexer]") {
  std::string IDENTIFIER = "a";

  Lexer lexer = Lexer("return " + IDENTIFIER + ";");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 3);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::RETURN);

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::IDENTIFIER);
  REQUIRE(static_cast<const IdentifierToken&>(tokenContainer.view(1)).name ==
          IDENTIFIER);

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
}

TEST_CASE("Print number", "[lexer]") {
  Lexer lexer = Lexer("print 0;");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 3);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::PRINT);

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::NUMBER);
  REQUIRE(static_cast<const NumberToken&>(tokenContainer.view(1)).value == 0);

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
}

TEST_CASE("Print identifier", "[lexer]") {
  std::string IDENTIFIER = "a";

  Lexer lexer = Lexer("print " + IDENTIFIER + ";");

  TokenContainer tokenContainer = lexer.makeTokenList();

  REQUIRE(tokenContainer.getCount() == 3);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::PRINT);

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::IDENTIFIER);
  REQUIRE(static_cast<const IdentifierToken&>(tokenContainer.view(1)).name ==
          IDENTIFIER);

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
}