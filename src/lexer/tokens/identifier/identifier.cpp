#pragma once

#include "./identifier.hpp"

Identifier::Identifier(const std::string name)
    : Token(TokenType::IDENTIFIER), name(name) {};