#include "syntax_analyser/statement/return/return.hpp"
#include "./return.hpp"
#include "syntax_analyser/statement/value/value.hpp"

ReturnStatement::ReturnStatement(std::unique_ptr<StatementValue> value)
    : Statement(StatementType::RETURN), value(std::move(value)) {}