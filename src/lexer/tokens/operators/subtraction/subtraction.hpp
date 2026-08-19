#pragma once

#include "../operator.hpp"
#include "lexer/tokens/token.hpp"

class SubtractionToken : public OperatorToken {
public:
  SubtractionToken(const TokenMetadata& metadata);
};