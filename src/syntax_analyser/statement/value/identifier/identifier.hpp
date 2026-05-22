#pragma once

#include "syntax_analyser/statement/value/value.hpp"
#include <string>

class IdentifierValue : public StatementValue {
public:
  const std::string name;
  IdentifierValue(const std::string name);
};