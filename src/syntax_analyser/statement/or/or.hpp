
#pragma once

#include "lexer/tokens/operators/boolean/and/and.hpp"
#include "lexer/tokens/operators/boolean/or/or.hpp"
#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class OrStatement : public Statement {
public:
  const OtherToken identifier;
  const OtherToken lhs;
  const OrToken orToken;
  const OtherToken rhs;

  OrStatement(const OtherToken& identifier, const OtherToken& lhs,
              const OrToken& orToken, const OtherToken& rhs)
      : Statement(StatementType::OR), identifier(identifier), lhs(lhs),
        orToken(orToken), rhs(rhs) {};
};