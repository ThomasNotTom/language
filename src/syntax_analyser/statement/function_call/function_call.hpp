#pragma once

#include <vector>

#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class FunctionCallStatement : public Statement {
public:
  const OtherToken identifier;
  const std::vector<OtherToken> parameters;

  FunctionCallStatement(const OtherToken& identifier,
                        const std::vector<OtherToken>& parameters)
      : Statement(StatementType::FUNCTION_CALL), identifier(identifier),
        parameters(parameters) {};
};