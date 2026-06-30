#include "./operator.hpp"

OperatorToken::OperatorToken(const OperatorType operatorType)
    : Token(TokenType::OPERATOR), operatorType(operatorType) {}