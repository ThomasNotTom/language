#pragma once

#include "./token_type.hpp"

class Token {
private:
  const TokenType tokenType;

public:
  Token(const TokenType tokenType);

  TokenType getType() const;
};