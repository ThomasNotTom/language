#pragma once

#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class AssignmentStatement : public Statement {
public:
  const OtherToken identifier;
  const OtherToken value;

  AssignmentStatement(const OtherToken& identifier, const OtherToken& value)
      : Statement(StatementType::ASSIGNMENT), identifier(identifier),
        value(value) {};
};