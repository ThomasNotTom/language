#pragma once

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class SubtractionStatement : public Statement {
public:
  const OtherToken identifier;
  const OtherToken lhs;
  const OperatorToken subtractOperator;
  const OtherToken rhs;

  SubtractionStatement(const OtherToken& identifier, const OtherToken& lhs,
                       const OperatorToken& subtractOperator,
                       const OtherToken& rhs)
      : Statement(StatementType::SUBTRACTION), identifier(identifier), lhs(lhs),
        subtractOperator(subtractOperator), rhs(rhs) {};
};