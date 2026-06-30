#pragma once

#include "./statement_type.hpp"

class Statement {
public:
  const StatementType statementType;

  Statement(const StatementType statementType);

  virtual ~Statement() = default;
};