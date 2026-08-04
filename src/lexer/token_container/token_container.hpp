#pragma once

#include <memory>
#include <vector>

#include "lexer/tokens/end_of_line/end_of_line.hpp"
#include "lexer/tokens/identifier/identifier.hpp"
#include "lexer/tokens/number/number.hpp"
#include "lexer/tokens/operators/addition/addition.hpp"
#include "lexer/tokens/operators/assignment/assignment.hpp"
#include "lexer/tokens/operators/subtraction/subtraction.hpp"
#include "lexer/tokens/primitives/uint16/uint16.hpp"
#include "lexer/tokens/primitives/uint32/uint32.hpp"
#include "lexer/tokens/primitives/uint64/uint64.hpp"
#include "lexer/tokens/primitives/uint8/uint8.hpp"
#include "lexer/tokens/print/print.hpp"
#include "lexer/tokens/return/return.hpp"
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

  void addSubtraction(const SubtractionToken& token) {
    tokens.push_back(std::make_unique<SubtractionToken>(token));
  }

  void addAssignment(const AssignmentToken& token) {
    tokens.push_back(std::make_unique<AssignmentToken>(token));
  }

  void addUint8(const Uint8Token& token) {
    tokens.push_back(std::make_unique<Uint8Token>(token));
  }

  void addUint16(const Uint16Token& token) {
    tokens.push_back(std::make_unique<Uint16Token>(token));
  }

  void addUint32(const Uint32Token& token) {
    tokens.push_back(std::make_unique<Uint32Token>(token));
  }

  void addUint64(const Uint64Token& token) {
    tokens.push_back(std::make_unique<Uint64Token>(token));
  }

  void addReturn(const ReturnToken& token) {
    tokens.push_back(std::make_unique<ReturnToken>(token));
  }

  void addPrint(const PrintToken& token) {
    tokens.push_back(std::make_unique<PrintToken>(token));
  }

  const Token& view(size_t index) const;
  size_t getCount() const;

  void print() const;
};