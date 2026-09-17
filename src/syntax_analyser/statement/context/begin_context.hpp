#include "syntax_analyser/statement/statement.hpp"

class BeginContexttatement : public Statement {
public:
  BeginContexttatement() : Statement(StatementType::CONTEXT_BEGIN) {};
};