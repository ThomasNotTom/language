#pragma once

#include <string>

#include "./string_converter.hpp"
#include "./token_container/token_container.hpp"
#include "./tokens/end_of_line/end_of_line.hpp"
#include "./tokens/identifier/identifier.hpp"
#include "./tokens/operators/assignment/assignment.hpp"
#include "./tokens/primitives/uint8/uint8.hpp"

#include "./tokens/operators/addition/addition.hpp"
#include "lexer/tokens/return/return.hpp"
#include "matcher.hpp"

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
          tokens.addUint8(Uint8Token());
        } else if (buffer == "=") {
          tokens.addAssignment(AssignmentToken());
        } else if (Matcher::isInt(buffer)) {
          tokens.addNumber(StringConverter::toInt(buffer));
        } else if (buffer == "+") {
          tokens.addAddition(AdditionToken());
        } else if (buffer == "return") {
          tokens.addReturn(ReturnToken());
        } else {
          tokens.addIdentifier(IdentifierToken(buffer));
        }

        buffer = "";
      }

      if (c == ';') {
        tokens.addEndOfLine(EndOfLineToken());
      }
    }

    return tokens;
  };
};