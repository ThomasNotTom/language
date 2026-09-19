#pragma once

#include "lexer/tokens/token.hpp"

class AdditionToken : public Token {
public:
  AdditionToken(const TokenMetadata& metadata);
};