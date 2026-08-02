#pragma once
#include <memory>

#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/value.hpp"

class PrintStatement : public Statement {
public:
  const std::unique_ptr<StatementValue> value;

  PrintStatement(std::unique_ptr<StatementValue> value);
};