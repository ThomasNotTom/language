#include "../token.hpp"
#include "../token_type.hpp"
#include "./primitive_type.hpp"

class Primitive : Token {
private:
  PrimitiveType primitiveType;

public:
  Primitive(const PrimitiveType type)
      : Token(TokenType::PRIMITIVE), primitiveType(type) {}
};