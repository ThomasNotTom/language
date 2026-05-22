#pragma once

#include "lexer/token_container/token_container.hpp"
#include "syntax_analyser/program/program.hpp"

class AbstractSyntaxTree {
private:
  const TokenContainer& tokenContainer;

public:
  AbstractSyntaxTree(const TokenContainer& tokenContainer);

  Program parse();
};