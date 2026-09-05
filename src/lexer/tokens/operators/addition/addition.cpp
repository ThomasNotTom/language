#include "./addition.hpp"

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

AdditionToken::AdditionToken(const TokenMetadata& metadata)
    : Token(TokenType::PLUS, metadata) {}