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
  REQUIRE(static_cast<const OtherToken&>(tokenContainer.view(0)).name == "a");

  REQUIRE(tokenContainer.view(2).tokenType == TokenType::END_OF_LINE);
};