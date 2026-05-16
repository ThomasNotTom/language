#pragma once

#include "../token.hpp"

#include "./operator_type.hpp"

class Operator : Token {
private:
  OperatorType operatorType;

public:
  Operator(const OperatorType type);
};