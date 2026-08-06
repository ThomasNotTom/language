#pragma once

#include <string>

#include "./string_converter.hpp"
#include "./tokens/end_of_line/end_of_line.hpp"
#include "./tokens/operators/assignment/assignment.hpp"
#include "./tokens/other.hpp"

#include "./tokens/operators/addition/addition.hpp"
#include "lexer/tokens/operators/subtraction/subtraction.hpp"
#include "lexer/tokens/print/print.hpp"
#include "lexer/tokens/print/print.hpp"
#include "lexer/token_container/token_container.hpp"
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
        if (buffer == "=") {
          tokens.addAssignment(AssignmentToken());
        } else if (buffer == "+") {
          tokens.addAddition(AdditionToken());
        } else if (buffer == "-") {
          tokens.addSubtraction(SubtractionToken());
        } else if (buffer == "return") {
          tokens.addReturn(ReturnToken());
        } else if (buffer == "print") {
          tokens.addPrint(PrintToken());
        } else if (buffer.size() != 0) {
          tokens.addOther(OtherToken(buffer));
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