#pragma once

#include "./number.hpp"

NumberToken::NumberToken(const int value)
    : Token(TokenType::NUMBER), value(value) {};