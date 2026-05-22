#include "./identifier.hpp"
#include "syntax_analyser/statement/value/value.hpp"

IdentifierValue::IdentifierValue(const std::string name)
    : StatementValue(StatementValueType::IDENTIFIER), name(name) {};