#pragma once

#include "./value_type.hpp"

class StatementValue {
public:
  const StatementValueType statementValueType;
  StatementValue(const StatementValueType statementValueType);
};