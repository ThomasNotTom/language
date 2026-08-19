#pragma once

#include "../operator.hpp"
#include "lexer/tokens/token.hpp"

class AdditionToken : public OperatorToken {
public:
  AdditionToken(const TokenMetadata& metadata);
};