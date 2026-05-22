#pragma once

#include "../token.hpp"

class NumberToken : public Token {
public:
  const int value;
  NumberToken(const int value);
};