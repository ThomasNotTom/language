#pragma once

#include "../statement.hpp"
#include "syntax_analyser/statement/other.hpp"

class InitialisationStatement : public Statement {
public:
  const OtherStatementValue type;
  const OtherStatementValue identifier;

  InitialisationStatement(const OtherStatementValue& type,
                          const OtherStatementValue& identifier)
      : Statement(StatementType::INITIALISATION), type(type),
        identifier(identifier) {};
};