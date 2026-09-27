
#pragma once

#include "lexer/tokens/operators/boolean/equality/equality.hpp"
#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class EqualityStatement : public Statement {
public:
  const OtherToken identifier;
  const OtherToken lhs;
  const EqualityToken equalityToken;
  const OtherToken rhs;

  EqualityStatement(const OtherToken& identifier, const OtherToken& lhs,
                    const EqualityToken& equalityToken, const OtherToken& rhs)
      : Statement(StatementType::EQUALITY), identifier(identifier), lhs(lhs),
        equalityToken(equalityToken), rhs(rhs) {};
};