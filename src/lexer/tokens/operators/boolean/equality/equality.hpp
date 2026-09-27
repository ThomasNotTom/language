#pragma once

#include "lexer/tokens/token.hpp"

class EqualityToken : public Token {
public:
  EqualityToken(const TokenMetadata& metadata);
};