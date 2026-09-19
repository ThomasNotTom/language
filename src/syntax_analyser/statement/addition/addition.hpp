#pragma once
#include "lexer/tokens/operators/addition/addition.hpp"
#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class AdditionStatement : public Statement {
public:
  const OtherToken identifier;
  const OtherToken lhs;
  const AdditionToken addOperator;
  const OtherToken rhs;

  AdditionStatement(const OtherToken& identifier, const OtherToken& lhs,
                    const AdditionToken& addOperator, const OtherToken& rhs)
      : Statement(StatementType::ADDITION), identifier(identifier), lhs(lhs),
        addOperator(addOperator), rhs(rhs) {};
};