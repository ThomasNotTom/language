#pragma once

#include "../primitives/primitive.hpp"
#include "../statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include <memory>

class InitialisationStatement : public Statement {
public:
  const StatementPrimitiveType type;
  const std::unique_ptr<IdentifierValue> identifier;

  InitialisationStatement(StatementPrimitiveType type,
                          std::unique_ptr<IdentifierValue> identifier);
};