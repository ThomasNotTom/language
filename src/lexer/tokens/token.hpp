#pragma once

#include "./token_type.hpp"

struct TokenMetadata {
  uint64_t line;
  uint64_t startIndex;
  uint64_t endIndex;
};

class Token {
public:
  const TokenMetadata metadata;
  const TokenType tokenType;
  Token(const TokenType tokenType, const TokenMetadata& metadata);

  virtual ~Token() = default;
};