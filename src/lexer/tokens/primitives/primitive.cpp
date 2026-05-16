#include "./primitive.hpp"
#include "./primitive_type.hpp"

Primitive::Primitive(const PrimitiveType primitiveType)
    : Token(TokenType::PRIMITIVE), primitiveType(primitiveType) {}