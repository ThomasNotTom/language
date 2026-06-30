#pragma once

#include "../statement.hpp"
#include "syntax_analyser/statement/value/value.hpp"
#include <memory>

class ReturnStatement : public Statement {
public:
  const std::unique_ptr<StatementValue> value;

  ReturnStatement(std::unique_ptr<StatementValue> value);
};