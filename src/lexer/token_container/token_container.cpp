#include <iostream>

#include "../tokens/other.hpp"
#include "../tokens/token_type.hpp"
#include "./token_container.hpp"

#include "../tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"

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

      case OPERATOR: {
        const OperatorToken& op = static_cast<const OperatorToken&>(token);
        std::string out = "OPERATOR(";

        switch (op.operatorType) {
          case ASSIGNMENT: {
            out += "=";
            break;
          }

          case ADDITION: {
            out += "+";
            break;
          }

          case SUBTRACTION: {
            out += "-";
            break;
          }
        }
        out += ")";

        std::cout << out << "\n";
        break;
      }

      case RETURN: {
        std::cout << "RETURN\n";
        break;
      }

      case PRINT: {
        std::cout << "PRINT\n";
        break;
      }

      case OTHER: {
        const OtherToken& other = static_cast<const OtherToken&>(token);

        std::cout << "OTHER(\"" << other.name << "\")\n";
        break;
      }
    }
  }
}