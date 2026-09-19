#include "./assignment.hpp"

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

AssignmentToken::AssignmentToken(const TokenMetadata& metadata)
    : Token(TokenType::EQUALS, metadata) {};