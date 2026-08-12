#include "./addition.hpp"

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"

AdditionToken::AdditionToken() : OperatorToken(OperatorType::ADDITION) {}