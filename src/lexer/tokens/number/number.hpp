#pragma once

#include "../token.hpp"

class NumberToken : public Token {
public:
  const unsigned long long value;
  NumberToken(const unsigned long long value);
};