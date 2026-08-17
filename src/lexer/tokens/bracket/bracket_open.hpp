#pragma once

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

class BracketOpen : public Token {
public:
  BracketOpen(const TokenMetadata& metadata)
      : Token(TokenType::BRACKET_OPEN, metadata) {};
};