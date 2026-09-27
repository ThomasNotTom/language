#pragma once

#include "lexer/tokens/token.hpp"

class OrToken : public Token {
public:
  OrToken(const TokenMetadata& metadata);
};