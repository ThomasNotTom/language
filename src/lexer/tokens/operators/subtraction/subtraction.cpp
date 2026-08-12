#include "./subtraction.hpp"

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/operators/subtraction/subtraction.hpp"

SubtractionToken::SubtractionToken()
    : OperatorToken(OperatorType::SUBTRACTION) {}