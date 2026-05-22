#include "./number.hpp"
#include "syntax_analyser/statement/value/value.hpp"

NumberValue::NumberValue(int value)
    : StatementValue(StatementValueType::NUMBER), value(value) {}