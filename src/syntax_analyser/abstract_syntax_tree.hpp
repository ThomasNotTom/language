#pragma once

#include "lexer/token_container/token_container.hpp"
#include "syntax_analyser/program/program.hpp"

class AbstractSyntaxTree {
private:
  const TokenContainer& tokenContainer;

public:
  AbstractSyntaxTree(const TokenContainer& tokenContainer);
  std::vector<std::unique_ptr<Statement>>
  leftToRightParse(std::vector<std::reference_wrapper<const Token>> tokens);

  std::vector<std::vector<std::reference_wrapper<const Token>>>
  splitToLines(const TokenContainer& fullTokens);

  Program parse();
};