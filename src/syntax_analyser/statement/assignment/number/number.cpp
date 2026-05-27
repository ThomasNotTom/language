#include "./number.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

AssignmentNumberStatement::AssignmentNumberStatement(IdentifierValue identifier,
                                                     NumberValue value)
    : AssignmentStatement(AssignmentType::NUMBER, identifier), value(value) {};