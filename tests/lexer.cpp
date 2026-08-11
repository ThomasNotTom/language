#include "lexer/lexer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_test_failure_exception.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <csignal>
#include <cstdlib>

#include "generation/generator.hpp"
#include "io/file_reader.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/token_type.hpp"

TEST_CASE("Empty string", "[lexer]") {
  Lexer lexer = Lexer("");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 0);
};

TEST_CASE("Single end of line", "[lexer]") {
  Lexer lexer = Lexer(";");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 1);
  REQUIRE(tokenContainer.view(0).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Variable initialisation", "[lexer]") {
  Lexer lexer = Lexer("uint8 a;");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 3);

  REQUIRE(tokenContainer.view(0).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(0)).name ==
          "uint8");

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(1)).name == "a");

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Variable initialisation and assignment", "[lexer]") {
  Lexer lexer = Lexer("uint8 a = 0;");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 5);

  REQUIRE(tokenContainer.view(0).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(0)).name ==
          "uint8");

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(1)).name == "a");

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::OPERATOR);
  REQUIRE(
      static_cast<const OperatorToken&>(tokenContainer.view(2)).operatorType ==
      OperatorType::ASSIGNMENT);

  REQUIRE(tokenContainer.view(3).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(3)).name == "0");

  REQUIRE(tokenContainer.view(4).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Variable assignment", "[lexer]") {
  Lexer lexer = Lexer("a = 0;");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 4);

  REQUIRE(tokenContainer.view(0).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(0)).name == "a");

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::OPERATOR);
  REQUIRE(
      static_cast<const OperatorToken&>(tokenContainer.view(1)).operatorType ==
      OperatorType::ASSIGNMENT);

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(2)).name == "0");

  REQUIRE(tokenContainer.view(3).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Variable initialisation and assignment with arithmetic", "[lexer]") {
  Lexer lexer = Lexer("uint8 a = 0 + 1;");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 7);

  REQUIRE(tokenContainer.view(0).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(0)).name ==
          "uint8");

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(1)).name == "a");

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::OPERATOR);
  REQUIRE(
      static_cast<const OperatorToken&>(tokenContainer.view(2)).operatorType ==
      OperatorType::ASSIGNMENT);

  REQUIRE(tokenContainer.view(3).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(3)).name == "0");

  REQUIRE(tokenContainer.view(4).tokenType == TokenType::OPERATOR);
  REQUIRE(
      static_cast<const OperatorToken&>(tokenContainer.view(4)).operatorType ==
      OperatorType::ADDITION);

  REQUIRE(tokenContainer.view(5).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(5)).name == "1");

  REQUIRE(tokenContainer.view(6).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Variable assignment with arithmetic", "[lexer]") {
  Lexer lexer = Lexer("a = 0 + 1;");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 6);

  REQUIRE(tokenContainer.view(0).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(0)).name == "a");

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::OPERATOR);
  REQUIRE(
      static_cast<const OperatorToken&>(tokenContainer.view(1)).operatorType ==
      OperatorType::ASSIGNMENT);

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(2)).name == "0");

  REQUIRE(tokenContainer.view(3).tokenType == TokenType::OPERATOR);
  REQUIRE(
      static_cast<const OperatorToken&>(tokenContainer.view(3)).operatorType ==
      OperatorType::ADDITION);

  REQUIRE(tokenContainer.view(4).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(4)).name == "1");

  REQUIRE(tokenContainer.view(5).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Print number", "[lexer]") {
  Lexer lexer = Lexer("print 1;");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 3);

  REQUIRE(tokenContainer.view(0).tokenType == TokenType::PRINT);

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(1)).name == "1");

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
};

TEST_CASE("Return number", "[lexer]") {
  Lexer lexer = Lexer("return 1;");
  TokenContainer tokenContainer = lexer.makeTokenList();
  REQUIRE(tokenContainer.getCount() == 3);

  REQUIRE(tokenContainer.view(0).tokenType == TokenType::RETURN);

  REQUIRE(tokenContainer.view(1).tokenType == TokenType::OTHER);
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(1)).name == "1");

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
};