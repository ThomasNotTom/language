#include "syntax_analyser/statement/return/return.hpp"
#include "./return.hpp"
#include "lexer/tokens/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"

ReturnStatement::ReturnStatement(std::unique_ptr<IdentifierValue> identifier)
    : Statement(StatementType::RETURN), identifier(std::move(identifier)) {}