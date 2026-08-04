#pragma once
#include <memory>

#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

class SubtractionStatement : public Statement {
public:
  const IdentifierValue identifier;
  const std::unique_ptr<StatementValue> lhs;
  const std::unique_ptr<StatementValue> rhs;

  SubtractionStatement(const IdentifierValue identifier,
                       std::unique_ptr<StatementValue> lhs,
                       std::unique_ptr<StatementValue> rhs)
      : Statement(StatementType::SUBTRACTION), identifier(identifier),
        lhs(std::move(lhs)), rhs(std::move(rhs)) {};
};