#include "syntax_analyser/statement/value/value.hpp"

class NumberValue : public StatementValue {
public:
  const int value;

  NumberValue(int value);
};