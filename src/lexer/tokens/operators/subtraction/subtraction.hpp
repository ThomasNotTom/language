#pragma once

#include "lexer/tokens/token.hpp"

class SubtractionToken : public Token {
public:
  SubtractionToken(const TokenMetadata& metadata);
};