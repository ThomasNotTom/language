#pragma once

#include "../statement.hpp"
#include "syntax_analyser/statement/other.hpp"

class ReturnStatement : public Statement {
public:
  const OtherStatementValue value;

  ReturnStatement(OtherStatementValue value)
      : Statement(StatementType::RETURN), value(std::move(value)) {};
};