#include "./primitive.hpp"
#include "./primitive_type.hpp"

PrimitiveToken::PrimitiveToken(const PrimitiveType primitiveType)
    : Token(TokenType::PRIMITIVE), primitiveType(primitiveType) {}