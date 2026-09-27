#include "./equality.hpp"

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

EqualityToken::EqualityToken(const TokenMetadata& metadata)
    : Token(TokenType::EQUALITY, metadata) {}