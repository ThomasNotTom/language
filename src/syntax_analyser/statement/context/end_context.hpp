#include "syntax_analyser/statement/statement.hpp"

class EndContexttatement : public Statement {
public:
  EndContexttatement() : Statement(StatementType::CONTEXT_END) {};
};