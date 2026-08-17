#pragma once

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

class BracketClose : public Token {
public:
  BracketClose(const TokenMetadata& metadata)
      : Token(TokenType::BRACKET_CLOSE, metadata) {};
};