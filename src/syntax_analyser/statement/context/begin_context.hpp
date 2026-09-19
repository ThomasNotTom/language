#include "syntax_analyser/statement/statement.hpp"

class BeginContextStatement : public Statement {
public:
  BeginContextStatement() : Statement(StatementType::CONTEXT_BEGIN) {};
};