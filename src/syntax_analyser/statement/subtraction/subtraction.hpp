#pragma once
#include <memory>

#include "syntax_analyser/statement/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class SubtractionStatement : public Statement {
public:
  const OtherStatementValue identifier;
  const OtherStatementValue lhs;
  const OtherStatementValue rhs;

  SubtractionStatement(const OtherStatementValue identifier,
                       OtherStatementValue lhs, OtherStatementValue rhs)
      : Statement(StatementType::SUBTRACTION), identifier(identifier), lhs(lhs),
        rhs(rhs) {};
};