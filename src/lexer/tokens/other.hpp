#pragma once

#include <string>

#include "./token.hpp"
#include "./token_type.hpp"

class OtherToken : public Token {
public:
  const std::string name;
  OtherToken(const std::string& name, const TokenMetadata& metadata)
      : Token(TokenType::OTHER, metadata), name(name) {};
};