#include "./number.hpp"

NumberToken::NumberToken(const unsigned long long value)
    : Token(TokenType::NUMBER), value(value) {};