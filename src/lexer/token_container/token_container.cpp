#include "./token_container.hpp"

#include <cstddef>
#include <iostream>
#include <string>

#include "../tokens/operators/operator.hpp"
#include "../tokens/other.hpp"
#include "../tokens/token.hpp"
#include "../tokens/token_type.hpp"
#include "lexer/tokens/operators/operator_type.hpp"

TokenContainer::TokenContainer() {};

const Token& TokenContainer::view(size_t index) const {
  return *this->tokens[index].get();
}

size_t TokenContainer::getCount() const { return this->tokens.size(); }

std::string
TokenContainer::tokenMetadataToString(const TokenMetadata& metadata) {
  std::string out = "";
  out += "line: " + std::to_string(metadata.line) + ", column: ";
  if (metadata.startIndex == metadata.endIndex) {
    out += std::to_string(metadata.startIndex);
  } else {
    out += std::to_string(metadata.startIndex) + "-" +
           std::to_string(metadata.endIndex);
  }
  return out;
}

void TokenContainer::print() const {
  std::cout << "-- Token Container --\n";
  std::cout << "count: " << this->getCount() << "\n";
  std::cout << "tokens: " << "\n";
  for (size_t i = 0; i < this->getCount(); i++) {
    const Token& token = this->view(i);

    switch (token.tokenType) {
      case END_OF_LINE: {
        std::cout << "END_OF_LINE";
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

        std::cout << out;
        break;
      }

      case OTHER: {
        const OtherToken& other = static_cast<const OtherToken&>(token);

        std::cout << "OTHER(\"" << other.name << "\")";
        break;
      }

      case BRACKET_OPEN: {
        std::cout << "BRACKET_OPEN";
        break;
      }

      case BRACKET_CLOSE: {
        std::cout << "BRACKET_CLOSE";
        break;
      }
    }

    std::cout << "  # " << TokenContainer::tokenMetadataToString(token.metadata)
              << "\n";
  }
}