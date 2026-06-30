#include <iostream>

#include "../tokens/identifier/identifier.hpp"
#include "../tokens/token_type.hpp"
#include "./token_container.hpp"

#include "../tokens/number/number.hpp"
#include "../tokens/operators/operator.hpp"
#include "../tokens/primitives/primitive.hpp"

TokenContainer::TokenContainer() {};

const Token& TokenContainer::view(size_t index) const {
  return *this->tokens[index].get();
}

size_t TokenContainer::getCount() const { return this->tokens.size(); }

void TokenContainer::print() const {
  std::cout << "-- Token Container --\n";
  std::cout << "count: " << this->getCount() << "\n";
  std::cout << "tokens: " << "\n";
  for (size_t i = 0; i < this->getCount(); i++) {
    const Token& token = this->view(i);

    switch (token.tokenType) {
      case END_OF_LINE: {
        std::cout << "END_OF_LINE\n";
        break;
      }

      case PRIMITIVE: {
        const PrimitiveToken& primitive =
            static_cast<const PrimitiveToken&>(token);

        switch (primitive.primitiveType) {
          case PrimitiveType::UINT8: {
            std::cout << "PRIMITIVE(UINT8)\n";
            break;
          }
        }
        break;
      }

      case OPERATOR: {
        const OperatorToken& op = static_cast<const OperatorToken&>(token);

        switch (op.operatorType) {
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
        const IdentifierToken& identifier =
            static_cast<const IdentifierToken&>(token);

        std::cout << "IDENTIFIER(\"" << identifier.name << "\")\n";
        break;
      }

      case NUMBER: {
        const NumberToken& number = static_cast<const NumberToken&>(token);

        std::cout << "NUMBER(" << number.value << ")\n";
        break;
      }

      case RETURN: {
        std::cout << "RETURN\n";
      }
    }
  }
}