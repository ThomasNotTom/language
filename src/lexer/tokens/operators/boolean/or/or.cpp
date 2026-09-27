#include "./or.hpp"

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

OrToken::OrToken(const TokenMetadata& metadata)
    : Token(TokenType::OR, metadata) {}