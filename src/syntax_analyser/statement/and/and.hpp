
#pragma once

#include "lexer/tokens/operators/boolean/and/and.hpp"
#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class AndStatement : public Statement {
public:
  const OtherToken identifier;
  const OtherToken lhs;
  const OrToken andToken;
  const OtherToken rhs;

  AndStatement(const OtherToken& identifier, const OtherToken& lhs,
               const OrToken& andToken, const OtherToken& rhs)
      : Statement(StatementType::AND), identifier(identifier), lhs(lhs),
        andToken(andToken), rhs(rhs) {};
};