#pragma once

#include "lexer/tokens/token.hpp"

class NotToken : public Token {
public:
  NotToken(const TokenMetadata& metadata);
};