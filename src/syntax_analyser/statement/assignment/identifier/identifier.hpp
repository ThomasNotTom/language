#pragma once

#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

class AssignmentIdentifierStatement : public AssignmentStatement {
public:
  const IdentifierValue value;

  AssignmentIdentifierStatement(const IdentifierValue identifier,
                                const IdentifierValue value);
};