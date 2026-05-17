#pragma once

#include "./token_type.hpp"

class Token {
public:
  const TokenType tokenType;
  Token(const TokenType tokenType);
};