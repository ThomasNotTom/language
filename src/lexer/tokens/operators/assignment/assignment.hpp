#pragma once

#include "../operator.hpp"
#include "lexer/tokens/token.hpp"

class AssignmentToken : public OperatorToken {
public:
  AssignmentToken(const TokenMetadata& metadata);
};