#include "./token.hpp"

#include "token_type.hpp"

Token::Token(const TokenType tokenType, const TokenMetadata& metadata)
    : tokenType(tokenType), metadata(metadata) {};
