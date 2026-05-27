#pragma once

#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/number/number.hpp"

class AssignmentNumberStatement : public AssignmentStatement {
public:
  const NumberValue value;

  AssignmentNumberStatement(const IdentifierValue identifier,
                            const NumberValue value);
};