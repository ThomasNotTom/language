#pragma once

#include "lexer/tokens/token.hpp"

class AssignmentToken : public Token {
public:
  AssignmentToken(const TokenMetadata& metadata);
};