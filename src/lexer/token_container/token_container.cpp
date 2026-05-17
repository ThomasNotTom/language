#include <iostream>

#include "../tokens/identifier/identifier.hpp"
#include "../tokens/operators/operator.hpp"
#include "../tokens/token_type.hpp"
#include "./token_container.hpp"

#include "../tokens/number/number.hpp"
#include "../tokens/primitives/primitive.hpp"

TokenContainer::TokenContainer() {};

Token* TokenContainer::get(size_t index) const {
  return this->tokens[index].get();
}

size_t TokenContainer::getCount() const { return this->tokens.size(); }

void TokenContainer::print() const {
  for (size_t i = 0; i < this->getCount(); i++) {
    Token* token = this->get(i);

    switch (token->tokenType) {
      case END_OF_LINE: {
        std::cout << "END_OF_LINE\n";
        break;
      }

      case PRIMITIVE: {
        Primitive* primitive = (Primitive*)token;

        switch (primitive->primitiveType) {
          case UINT8: {
            std::cout << "PRIMITIVE(UINT8)\n";
            break;
          }
        }
        break;
      }

      case OPERATOR: {
        Operator* op = (Operator*)token;

        switch (op->operatorType) {
          case ASSIGNMENT: {
            std::cout << "OPERATOR(=)\n";
            break;
          }

          case ADDITION: {
            std::cout << "OPERATOR(+)\n";
            break;
          }
        }
        break;
      }

      case IDENTIFIER: {
        Identifier* identifier = (Identifier*)token;

        std::cout << "IDENTIFIER(\"" << identifier->name << "\")\n";
        break;
      }

      case NUMBER: {
        Number* number = (Number*)token;

        std::cout << "NUMBER(" << number->value << ")\n";
        break;
      }
    }
  }
}