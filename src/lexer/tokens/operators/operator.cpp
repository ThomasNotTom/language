#include "./operator.hpp"

#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

OperatorToken::OperatorToken(const OperatorType operatorType,
                             const TokenMetadata& metadata)
    : Token(TokenType::OPERATOR, metadata), operatorType(operatorType) {}