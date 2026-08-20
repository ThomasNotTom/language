#pragma once

#include "io/program_text.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/other.hpp"
#include "syntax_analyser/program/program.hpp"

class AbstractSyntaxTree {
private:
  const TokenContainer& tokenContainer;
  const ProgramText& programText;

public:
  AbstractSyntaxTree(const TokenContainer& tokenContainer, const ProgramText& programText);
  std::vector<std::unique_ptr<Statement>>
  leftToRightParse(std::vector<std::reference_wrapper<const Token>> tokens,
                   const OtherToken& outToken);

  std::vector<std::vector<std::reference_wrapper<const Token>>>
  splitToLines(const TokenContainer& fullTokens);

  Program parse();
};