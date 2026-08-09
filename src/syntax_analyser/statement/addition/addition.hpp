#pragma once
#include <iostream>
#include <memory>

#include "syntax_analyser/statement/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class AdditionStatement : public Statement {
public:
  const OtherStatementValue identifier;
  const OtherStatementValue lhs;
  const OtherStatementValue rhs;

  AdditionStatement(const OtherStatementValue identifier,
                    OtherStatementValue lhs, OtherStatementValue rhs)
      : Statement(StatementType::ADDITION), identifier(identifier), lhs(lhs),
        rhs(rhs) {};
};