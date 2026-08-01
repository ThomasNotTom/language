#include "syntax_analyser/statement/print/print.hpp"
#include "syntax_analyser/statement/value/value.hpp"
PrintStatement::PrintStatement(std::unique_ptr<StatementValue> value)
    : Statement(StatementType::PRINT), value(std::move(value)) {}