#pragma once

#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

class AdditionStatement : public Statement {
public:
  const IdentifierValue identifier;
  const std::unique_ptr<StatementValue> lhs;
  const std::unique_ptr<StatementValue> rhs;

  AdditionStatement(const IdentifierValue identifier,
                    std::unique_ptr<StatementValue> lhs,
                    std::unique_ptr<StatementValue> rhs)
      : Statement(StatementType::ADDITION), identifier(identifier),
        lhs(std::move(lhs)), rhs(std::move(rhs)) {};
};