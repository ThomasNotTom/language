#pragma once

#include <string>

#include "../token.hpp"

class Identifier : public Token {
public:
  const std::string name;
  Identifier(const std::string name);
};