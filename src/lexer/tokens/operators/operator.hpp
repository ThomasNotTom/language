#pragma once

#include "../token.hpp"

#include "./operator_type.hpp"

class OperatorToken : public Token {
public:
  const OperatorType operatorType;
  OperatorToken(const OperatorType type);
};