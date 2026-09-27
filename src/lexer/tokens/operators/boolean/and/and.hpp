#pragma once

#include "lexer/tokens/token.hpp"

class AndToken : public Token {
public:
  AndToken(const TokenMetadata& metadata);
};