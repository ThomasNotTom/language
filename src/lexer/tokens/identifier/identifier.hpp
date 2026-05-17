#pragma once

#include <string>

#include "../token.hpp"

class IdentifierToken : public Token {
public:
  const std::string name;
  IdentifierToken(const std::string name);
};