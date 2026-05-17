#include "./identifier.hpp"

IdentifierToken::IdentifierToken(const std::string name)
    : Token(TokenType::IDENTIFIER), name(name) {};