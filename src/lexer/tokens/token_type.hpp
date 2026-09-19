#pragma once

#include <cstdint>
#include <string>

enum TokenType : uint8_t {
  END_OF_LINE = 1,
  PLUS = 2,
  MINUS = 3,
  EQUALS = 4,
  BRACKET_OPEN = 5,
  BRACKET_CLOSE = 6,
  OTHER = 7,
};

class TokenChecker {
public:
  static bool isOperator(const TokenType& tokenType) {
    return tokenType == TokenType::PLUS || tokenType == TokenType::MINUS;
  }

  static std::string getOperatorTypes() {
    return std::to_string(TokenType::PLUS) + ", " +
           std::to_string(TokenType::MINUS);
  }
};