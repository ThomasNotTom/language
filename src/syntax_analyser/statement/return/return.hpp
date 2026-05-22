#pragma once

#include "../statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include <memory>

class ReturnStatement : public Statement {
public:
  const std::unique_ptr<IdentifierValue> identifier;

  ReturnStatement(std::unique_ptr<IdentifierValue> identifier);
};