#include "./assignment.hpp"

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/token.hpp"

AssignmentToken::AssignmentToken(const TokenMetadata& metadata)
    : OperatorToken(OperatorType::ASSIGNMENT, metadata) {};