#include "./not.hpp"

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

NotToken::NotToken(const TokenMetadata& metadata)
    : Token(TokenType::NOT, metadata) {}