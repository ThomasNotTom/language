#include "./assignment.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

AssignmentStatement::AssignmentStatement(IdentifierValue identifier,
                                         NumberValue value)
    : Statement(StatementType::ASSIGNMENT), identifier(identifier),
      value(value) {};