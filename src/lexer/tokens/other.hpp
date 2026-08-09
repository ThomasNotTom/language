#pragma once

#include "./token.hpp"
#include "./token_type.hpp"
#include <string>

class OtherToken : public Token {
public:
  const std::string name;
  OtherToken(const std::string name) : Token(TokenType::OTHER), name(name) {};
};