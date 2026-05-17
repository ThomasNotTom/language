#pragma once

#include <memory>
#include <vector>

#include "lexer/tokens/end_of_line/end_of_line.hpp"
#include "lexer/tokens/identifier/identifier.hpp"
#include "lexer/tokens/number/number.hpp"
#include "lexer/tokens/operators/addition/addition.hpp"
#include "lexer/tokens/operators/assignment/assignment.hpp"
#include "lexer/tokens/primitives/uint8/uint8.hpp"
#include "lexer/tokens/token.hpp"

class TokenContainer {
private:
  std::vector<std::unique_ptr<Token>> tokens;

public:
  TokenContainer();

  void addEndOfLine(const EndOfLineToken& token) {
    tokens.push_back(std::make_unique<EndOfLineToken>(token));
  }

  void addIdentifier(const IdentifierToken& token) {
    tokens.push_back(std::make_unique<IdentifierToken>(token));
  }

  void addNumber(const NumberToken& token) {
    tokens.push_back(std::make_unique<NumberToken>(token));
  }

  void addAddition(const AdditionToken& token) {
    tokens.push_back(std::make_unique<AdditionToken>(token));
  }

  void addAssignment(const AssignmentToken& token) {
    tokens.push_back(std::make_unique<AssignmentToken>(token));
  }

  void addUint8(const Uint8Token& token) {
    tokens.push_back(std::make_unique<Uint8Token>(token));
  }

  Token* get(size_t index) const;
  size_t getCount() const;

  void print() const;
};