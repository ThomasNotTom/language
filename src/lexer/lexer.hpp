#pragma once

#include <iostream>
#include <string>

#include "./tokens/end_of_line/end_of_line.hpp"
#include "./tokens/operators/addition/addition.hpp"
#include "./tokens/operators/assignment/assignment.hpp"
#include "./tokens/other.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/operators/subtraction/subtraction.hpp"
#include "lexer/tokens/token.hpp"

class Lexer {
private:
  const std::string input;

public:
  Lexer(const std::string& input) : input(input) {};

  TokenContainer makeTokenList() const {
    uint64_t index = 0;
    uint64_t bufferStartColumn = 1;
    uint64_t lineNumber = 1;
    bool bufferBegan = false;

    TokenContainer tokens;
    std::string buffer = "";

    for (char c : this->input) {
      index += 1;

      if (c == '\n') {
        lineNumber += 1;
        index = 0;
        continue;
      }

      if (c != ' ' && c != ';' && c != '(' && c != ')') {
        if (!bufferBegan) {
          bufferStartColumn = index;
          bufferBegan = true;
        }
        buffer += c;

        continue;
      }

      uint64_t endColumn = index - 1;
      if (buffer == "=") {
        tokens.addAssignment(AssignmentToken(
            TokenMetadata(lineNumber, bufferStartColumn, endColumn)));
      } else if (buffer == "+") {
        tokens.addAddition(AdditionToken(
            TokenMetadata(lineNumber, bufferStartColumn, endColumn)));
      } else if (buffer == "-") {
        tokens.addSubtraction(SubtractionToken(
            TokenMetadata(lineNumber, bufferStartColumn, endColumn)));
      } else if (buffer.size() != 0) {
        tokens.addOther(OtherToken(
            buffer, TokenMetadata(lineNumber, bufferStartColumn, endColumn)));
      }
      buffer = "";
      bufferBegan = false;

      if (c == '(') {
        tokens.addOpenBracket(
            TokenMetadata(lineNumber, bufferStartColumn, endColumn));
      } else if (c == ')') {
        tokens.addCloseBracket(
            TokenMetadata(lineNumber, bufferStartColumn, endColumn));
      }

      if (c == ';') {
        tokens.addEndOfLine(
            EndOfLineToken(TokenMetadata(lineNumber, index, index)));
      }
    }

    return tokens;
  };
};