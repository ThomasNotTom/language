#include "./subtraction.hpp"

#include "lexer/tokens/operators/subtraction/subtraction.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

SubtractionToken::SubtractionToken(const TokenMetadata& metadata)
    : Token(TokenType::MINUS, metadata) {}