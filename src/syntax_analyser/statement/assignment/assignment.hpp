#pragma once

#include "syntax_analyser/statement/assignment/assignment_type.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

class AssignmentStatement : public Statement {
public:
  const AssignmentType assignmentType;
  const IdentifierValue identifier;

  AssignmentStatement(const AssignmentType assignmentType,
                      const IdentifierValue identifier);
};