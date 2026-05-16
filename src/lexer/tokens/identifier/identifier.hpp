#pragma once

#include <string>

#include "../token.hpp"

class Identifier : Token {
private:
  const std::string name;

public:
  Identifier(const std::string name);
};