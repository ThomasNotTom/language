#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/statement.hpp"

AssignmentStatement::AssignmentStatement(const AssignmentType assignmentType,
                                         const IdentifierValue identifier)
    : Statement(StatementType::ASSIGNMENT), assignmentType(assignmentType),
      identifier(identifier) {}