#include "./identifier.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

AssignmentIdentifierStatement::AssignmentIdentifierStatement(
    IdentifierValue identifier, IdentifierValue value)
    : AssignmentStatement(AssignmentType::IDENTIFIER, identifier),
      value(value) {};