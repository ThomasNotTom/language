#pragma once

#include <regex>

#include <memory>
#include <string>

#include "./token_container/token_container.hpp"
#include "./tokens/end_of_line/end_of_line.hpp"
#include "./tokens/identifier/identifier.hpp"
#include "./tokens/number/number.hpp"
#include "./tokens/operators/assignment/assignment.hpp"
#include "./tokens/primitives/uint8/uint8.hpp"

#include "./tokens/operators/addition/addition.hpp"

class Lexer {
private:
  const std::string input;

public:
  Lexer(const std::string input) : input(input) {};

  TokenContainer makeTokenList() const {

    TokenContainer tokens;
    std::string buffer = "";

    for (char c : this->input) {
      if (c == '\n') {
        continue;
      }

      if (c != ' ' && c != ';') {
        buffer += c;
        continue;
      }

      if (c == ' ' || c == ';') {
        if (buffer == "uint8") {
          tokens.addToken(std::make_unique<Uint8>());
        } else if (buffer == "=") {
          tokens.addToken(std::make_unique<Assignment>());
        } else if (std::regex_match(buffer, std::regex("^-?[0-9]+$"))) {
          tokens.addToken(std::make_unique<Number>(std::stoi(buffer)));
        } else if (buffer == "+") {
          tokens.addToken(std::make_unique<Addition>());
        } else {
          tokens.addToken(std::make_unique<Identifier>(buffer));
        }

        buffer = "";
      }

      if (c == ';') {
        tokens.addToken(std::make_unique<EndOfLine>());
      }
    }

    return tokens;
  };
};