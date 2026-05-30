#include "./abstract_syntax_tree.hpp"
#include "lexer/tokens/identifier/identifier.hpp"
#include "lexer/tokens/number/number.hpp"
#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/primitives/primitive.hpp"
#include "lexer/tokens/primitives/primitive_type.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"
#include "syntax_analyser/program/program.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/identifier/identifier.hpp"
#include "syntax_analyser/statement/assignment/number/number.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/number/number.hpp"
#include "syntax_analyser/statement/value/value.hpp"
#include <cstddef>
#include <format>
#include <memory>
#include <stdexcept>
#include <vector>

AbstractSyntaxTree::AbstractSyntaxTree(const TokenContainer& tokenContainer)
    : tokenContainer(tokenContainer) {}

StatementPrimitiveType
AbstractSyntaxTree::getStatementPrimitiveTypeFromPrimitiveType(
    PrimitiveType primitiveType) {
  switch (primitiveType) {
    case PrimitiveType::UINT8: {
      return StatementPrimitiveType::UINT8;
    }
  }
}

std::unique_ptr<InitialisationStatement>
AbstractSyntaxTree::makeInitialisationStatement(PrimitiveType primitiveType,
                                                std::string identifierName) {
  StatementPrimitiveType statementPrimitiveType =
      this->getStatementPrimitiveTypeFromPrimitiveType(primitiveType);

  return std::make_unique<InitialisationStatement>(
      statementPrimitiveType,
      std::make_unique<IdentifierValue>(identifierName));
}

std::vector<std::unique_ptr<Statement>> AbstractSyntaxTree::evaluateOperations(
    std::vector<std::reference_wrapper<const Token>> tokens,
    std::string outputIdentifier) {
  if (tokens.size() == 0) {
    throw std::runtime_error("No values to evaluate");
  }

  std::vector<std::unique_ptr<Statement>> statements;

  size_t tokensIndex = 0;

  // Set identfier = to whatever the left-most value is.

  switch (tokens[tokensIndex].get().tokenType) {
    case TokenType::NUMBER: {
      const NumberToken& numberToken =
          static_cast<const NumberToken&>(tokens[tokensIndex].get());

      statements.push_back(std::make_unique<AssignmentNumberStatement>(
          outputIdentifier, numberToken.value));
      break;
    }

    case TokenType::IDENTIFIER: {
      const IdentifierToken& identfierToken =
          static_cast<const IdentifierToken&>(tokens[tokensIndex].get());

      statements.push_back(std::make_unique<AssignmentIdentifierStatement>(
          outputIdentifier, identfierToken.name));
      break;
    }

    default: {
      throw std::runtime_error("Type must be a value or an identifier");
    }
  }

  tokensIndex += 1;

  while (tokensIndex < tokens.size()) {
    const Token& firstToken =
        static_cast<const Token&>(tokens[tokensIndex].get());

    if (firstToken.tokenType != TokenType::OPERATOR) {
      throw std::runtime_error("Token adjacent to value must be an operator");
    }

    const OperatorToken& firstTokenOperator =
        static_cast<const OperatorToken&>(firstToken);

    if (firstTokenOperator.operatorType != OperatorType::ADDITION) {
      throw std::format_error("Only `+` operator is implemented");
    }

    const Token& secondToken =
        static_cast<const Token&>(tokens[tokensIndex + 1].get());

    std::cout << secondToken.tokenType << "\n";

    if (secondToken.tokenType != TokenType::NUMBER &&
        secondToken.tokenType != TokenType::IDENTIFIER) {
      throw std::runtime_error(
          "Token adjacent to operator must be an identifier or a value");
    }

    if (secondToken.tokenType == TokenType::NUMBER) {
      statements.push_back(std::make_unique<AdditionStatement>(
          IdentifierValue(outputIdentifier),
          std::make_unique<IdentifierValue>(outputIdentifier),
          std::make_unique<NumberValue>(
              (static_cast<const NumberToken&>(secondToken)).value)));

      std::cout << outputIdentifier << " + "
                << (static_cast<const NumberToken&>(secondToken)).value << "\n";
    }

    if (secondToken.tokenType == TokenType::IDENTIFIER) {
      statements.push_back(std::make_unique<AdditionStatement>(
          IdentifierValue(outputIdentifier),
          std::make_unique<IdentifierValue>(outputIdentifier),
          std::make_unique<IdentifierValue>(
              static_cast<const IdentifierToken&>(secondToken).name)));
    }

    tokensIndex += 2;
  }

  return statements;
}

Program AbstractSyntaxTree::parse() {
  Program program;

  std::vector<std::reference_wrapper<const Token>> buffer;

  for (size_t i = 0; i < this->tokenContainer.getCount(); i++) {

    const Token& token = this->tokenContainer.view(i);
    buffer.push_back(token);

    if (token.tokenType == TokenType::END_OF_LINE) {
      if (buffer[0].get().tokenType == TokenType::PRIMITIVE &&
          buffer[1].get().tokenType == TokenType::IDENTIFIER &&
          buffer[2].get().tokenType == TokenType::OPERATOR) {

        const PrimitiveToken& primitive =
            static_cast<const PrimitiveToken&>(buffer[0].get());
        const IdentifierToken& identifier =
            dynamic_cast<const IdentifierToken&>(buffer[1].get());
        const OperatorToken& oper =
            dynamic_cast<const OperatorToken&>(buffer[2].get());

        if (oper.operatorType != OperatorType::ASSIGNMENT) {
          throw std::runtime_error("Assignment must have `=` operator");
        }

        std::vector<std::reference_wrapper<const Token>> remainingTokens(
            buffer.begin() + 3, buffer.end() - 1);

        // Initialise
        program.addInitialisation(std::make_unique<InitialisationStatement>(
            this->getStatementPrimitiveTypeFromPrimitiveType(
                primitive.primitiveType),
            std::make_unique<IdentifierValue>(identifier.name)));

        // Calculate statements for RHS
        std::vector<std::unique_ptr<Statement>> statements =
            this->evaluateOperations(remainingTokens, identifier.name);

        // Append RHS calculations
        for (size_t i = 0; i < statements.size(); i++) {
          program.addAnyStatement(std::move(statements[i]));
        }
      }

      if (buffer[0].get().tokenType == TokenType::RETURN &&
          buffer[1].get().tokenType == TokenType::IDENTIFIER) {

        const IdentifierToken& identifier =
            static_cast<const IdentifierToken&>(buffer[1].get());

        std::unique_ptr<ReturnStatement> returnStatement =
            std::make_unique<ReturnStatement>(
                std::make_unique<IdentifierValue>(identifier.name));

        program.addReturn(std::move(returnStatement));
      }
      buffer.clear();
    }
  }
  std::cout << "program size: " << program.size() << "\n";
  return program;
}