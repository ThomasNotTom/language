#pragma once
#include <memory>

#include "syntax_analyser/statement/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class PrintStatement : public Statement {
public:
  const OtherStatementValue value;

  PrintStatement(OtherStatementValue value)
      : Statement(StatementType::PRINT), value(value) {};
};