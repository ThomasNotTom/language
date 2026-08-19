#include "./addition.hpp"

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/token.hpp"

AdditionToken::AdditionToken(const TokenMetadata& metadata)
    : OperatorToken(OperatorType::ADDITION, metadata) {}