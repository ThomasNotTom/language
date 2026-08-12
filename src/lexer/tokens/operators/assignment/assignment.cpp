#include "./assignment.hpp"

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"

AssignmentToken::AssignmentToken() : OperatorToken(OperatorType::ASSIGNMENT) {};