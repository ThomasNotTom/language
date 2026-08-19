#pragma once

#include "../statement.hpp"
#include "lexer/tokens/other.hpp"

class InitialisationStatement : public Statement {
public:
  const OtherToken type;
  const OtherToken identifier;

  InitialisationStatement(const OtherToken& type, const OtherToken& identifier)
      : Statement(StatementType::INITIALISATION), type(type),
        identifier(identifier) {};
};