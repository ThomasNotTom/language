#pragma once

#include "../statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/number/number.hpp"

class AssignmentStatement : public Statement {
public:
  const IdentifierValue identifier;
  const NumberValue value;

  AssignmentStatement(const IdentifierValue identifier,
                      const NumberValue value);
};