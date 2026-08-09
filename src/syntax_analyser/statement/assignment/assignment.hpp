#pragma once

#include "syntax_analyser/statement/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class AssignmentStatement : public Statement {
public:
  const OtherStatementValue identifier;
  const OtherStatementValue value;

  AssignmentStatement(OtherStatementValue identifier, OtherStatementValue value)
      : Statement(StatementType::ASSIGNMENT), identifier(identifier),
        value(value) {};
};