#pragma once

#include "./number.hpp"

Number::Number(const int value) : Token(TokenType::NUMBER), value(value) {};