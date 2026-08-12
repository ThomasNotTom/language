#include "./end_of_line.hpp"

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

EndOfLineToken::EndOfLineToken() : Token(TokenType::END_OF_LINE) {};