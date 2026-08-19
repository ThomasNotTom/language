#include "./subtraction.hpp"

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/operators/subtraction/subtraction.hpp"
#include "lexer/tokens/token.hpp"

SubtractionToken::SubtractionToken(const TokenMetadata& metadata)
    : OperatorToken(OperatorType::SUBTRACTION, metadata) {}