#include "./initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

InitialisationStatement::InitialisationStatement(
    StatementPrimitiveType type, std::unique_ptr<IdentifierValue> identifier)
    : Statement(StatementType::INITIALISATION), type(type),
      identifier(std::move(identifier)) {}