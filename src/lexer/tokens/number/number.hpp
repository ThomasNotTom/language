#pragma once

#include "../token.hpp"

class Number : public Token {
public:
  const int value;
  Number(const int value);
};