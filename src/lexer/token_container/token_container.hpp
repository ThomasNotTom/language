#pragma once

#include <memory>
#include <vector>

#include "../../lexer/tokens/token.hpp"

class TokenContainer {
private:
  std::vector<std::unique_ptr<Token>> tokens;

public:
  TokenContainer();

  void addToken(std::unique_ptr<Token> token) {
    tokens.push_back(std::move(token));
  }

  Token* get(size_t index) const;
  size_t getCount() const;

  void print() const;
};