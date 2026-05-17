#pragma once

#include "../token.hpp"

#include "./operator_type.hpp"

class Operator : public Token {
public:
  const OperatorType operatorType;
  Operator(const OperatorType type);
};