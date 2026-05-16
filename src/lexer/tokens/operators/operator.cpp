#include "./operator.hpp"

Operator::Operator(const OperatorType operatorType)
    : Token(TokenType::OPERATOR), operatorType(operatorType) {}