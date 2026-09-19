#include "syntax_analyser/statement/statement.hpp"

class EndContextStatement : public Statement {
public:
  EndContextStatement() : Statement(StatementType::CONTEXT_END) {};
};