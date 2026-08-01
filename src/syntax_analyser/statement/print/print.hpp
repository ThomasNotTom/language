#pragma once

#include "syntax_analyser/statement/assignment/assignment_type.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

class PrintStatement : public Statement {
public:
  const std::unique_ptr<StatementValue> value;

  PrintStatement(std::unique_ptr<StatementValue> value);
};