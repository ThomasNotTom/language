#pragma once

#include "../statement.hpp"
#include "syntax_analyser/statement/other.hpp"
#include <memory>

class InitialisationStatement : public Statement {
public:
  const OtherStatementValue type;
  const OtherStatementValue identifier;

  InitialisationStatement(OtherStatementValue type,
                          OtherStatementValue identifier)
      : Statement(StatementType::INITIALISATION), type(type),
        identifier(identifier) {};
};