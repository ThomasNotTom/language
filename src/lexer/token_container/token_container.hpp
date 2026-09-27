#pragma once

#include <memory>
#include <vector>

#include "lexer/tokens/bracket/bracket_close.hpp"
#include "lexer/tokens/bracket/bracket_open.hpp"
#include "lexer/tokens/end_of_line/end_of_line.hpp"
#include "lexer/tokens/operators/addition/addition.hpp"
#include "lexer/tokens/operators/assignment/assignment.hpp"
#include "lexer/tokens/operators/boolean/and/and.hpp"
#include "lexer/tokens/operators/boolean/equality/equality.hpp"
#include "lexer/tokens/operators/boolean/not/not.hpp"
#include "lexer/tokens/operators/boolean/or/or.hpp"
#include "lexer/tokens/operators/subtraction/subtraction.hpp"
#include "lexer/tokens/other.hpp"
#include "lexer/tokens/token.hpp"

class TokenContainer {
private:
  std::vector<std::unique_ptr<Token>> tokens;

public:
  TokenContainer();

  void addEndOfLine(const EndOfLineToken& token) {
    tokens.push_back(std::make_unique<EndOfLineToken>(token));
  }

  void addOther(const OtherToken& token) {
    tokens.push_back(std::make_unique<OtherToken>(token));
  }

  void addAddition(const AdditionToken& token) {
    tokens.push_back(std::make_unique<AdditionToken>(token));
  }

  void addSubtraction(const SubtractionToken& token) {
    tokens.push_back(std::make_unique<SubtractionToken>(token));
  }

  void addOpenBracket(const BracketOpen& token) {
    tokens.push_back(std::make_unique<BracketOpen>(token));
  }

  void addCloseBracket(const BracketClose& token) {
    tokens.push_back(std::make_unique<BracketClose>(token));
  }

  void addAssignment(const AssignmentToken& token) {
    tokens.push_back(std::make_unique<AssignmentToken>(token));
  }

  void addAnd(const AndToken& token) {
    tokens.push_back(std::make_unique<AndToken>(token));
  }

  void addOr(const OrToken& token) {
    tokens.push_back(std::make_unique<OrToken>(token));
  }

  void addNot(const NotToken& token) {
    tokens.push_back(std::make_unique<NotToken>(token));
  }

  void addEquality(const EqualityToken& token) {
    tokens.push_back(std::make_unique<EqualityToken>(token));
  }

  static std::string tokenMetadataToString(const TokenMetadata& metadata);

  const Token& view(size_t index) const;
  size_t getCount() const;

  void print() const;
};