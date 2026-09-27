
#pragma once

#include "lexer/tokens/operators/boolean/not/not.hpp"
#include "lexer/tokens/other.hpp"
#include "syntax_analyser/statement/statement.hpp"

class NotStatement : public Statement {
public:
  const OtherToken identifier;
  const NotToken notToken;
  const OtherToken value;

  NotStatement(const OtherToken& identifier, const NotToken& notToken,
               const OtherToken& value)
      : Statement(StatementType::NOT), identifier(identifier),
        notToken(notToken), value(value) {};
};