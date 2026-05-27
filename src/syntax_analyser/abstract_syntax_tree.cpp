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

std::vector<std::unique_ptr<Statement>>
AbstractSyntaxTree::evaluateOperations(std::vector<Token*> tokens,
                                       std::string outputIdentifier) {
  if (tokens.size() == 0) {
    throw std::runtime_error("No values to evaluate");
  }

  std::vector<std::unique_ptr<Statement>> statements;

  size_t tokensIndex = 0;

  // Set identfier = to whatever the left-most value is.

  switch (tokens[tokensIndex]->tokenType) {
    case TokenType::NUMBER: {
      NumberToken* numberToken = (NumberToken*)tokens[tokensIndex];

      statements.push_back(std::make_unique<AssignmentNumberStatement>(
          outputIdentifier, numberToken->value));
      break;
    }

    case TokenType::IDENTIFIER: {
      IdentifierToken* identfierToken = (IdentifierToken*)tokens[tokensIndex];

      statements.push_back(std::make_unique<AssignmentIdentifierStatement>(
          outputIdentifier, identfierToken->name));
      break;
    }

    default: {
      throw std::runtime_error("Type must be a value or an identifier");
    }
  }

  tokensIndex += 1;

  while (tokensIndex < tokens.size()) {
    Token* firstToken = tokens[tokensIndex];

    if (firstToken->tokenType != TokenType::OPERATOR) {
      throw std::runtime_error("Token adjacent to value must be an operator");
    }

    OperatorToken* firstTokenOperator = (OperatorToken*)firstToken;
    if (firstTokenOperator->operatorType != OperatorType::ADDITION) {
      throw std::format_error("Only `+` operator is implemented");
    }

    Token* secondToken = tokens[tokensIndex + 1];
    std::cout << secondToken->tokenType << "\n";

    if (secondToken->tokenType != TokenType::NUMBER &&
        secondToken->tokenType != TokenType::IDENTIFIER) {
      throw std::runtime_error(
          "Token adjacent to operator must be an identifier or a value");
    }

    if (secondToken->tokenType == TokenType::NUMBER) {
      statements.push_back(std::make_unique<AdditionStatement>(
          IdentifierValue(outputIdentifier),
          std::make_unique<IdentifierValue>(outputIdentifier),
          std::make_unique<NumberValue>(((NumberToken*)secondToken)->value)));

      std::cout << outputIdentifier << " + "
                << ((NumberToken*)secondToken)->value << "\n";
    }

    if (secondToken->tokenType == TokenType::IDENTIFIER) {
      statements.push_back(std::make_unique<AdditionStatement>(
          IdentifierValue(outputIdentifier),
          std::make_unique<IdentifierValue>(outputIdentifier),
          std::make_unique<IdentifierValue>(
              ((IdentifierToken*)secondToken)->name)));
    }

    tokensIndex += 2;
  }

  return statements;
}

Program AbstractSyntaxTree::parse() {
  Program program;

  std::vector<Token*> buffer;

  for (size_t i = 0; i < this->tokenContainer.getCount(); i++) {

    Token* token = this->tokenContainer.get(i);
    buffer.push_back(token);

    if (token->tokenType == TokenType::END_OF_LINE) {
      if (buffer[0]->tokenType == TokenType::PRIMITIVE &&
          buffer[1]->tokenType == TokenType::IDENTIFIER &&
          buffer[2]->tokenType == TokenType::OPERATOR) {

        PrimitiveToken* primitive = dynamic_cast<PrimitiveToken*>(buffer[0]);
        IdentifierToken* identifier = dynamic_cast<IdentifierToken*>(buffer[1]);
        OperatorToken* oper = dynamic_cast<OperatorToken*>(buffer[2]);

        if (oper->operatorType != OperatorType::ASSIGNMENT) {
          throw std::runtime_error("Assignment must have `=` operator");
        }

        std::vector<Token*> remainingTokens(buffer.begin() + 3,
                                            buffer.end() - 1);

        // Initialise
        program.addInitialisation(std::make_unique<InitialisationStatement>(
            this->getStatementPrimitiveTypeFromPrimitiveType(
                primitive->primitiveType),
            std::make_unique<IdentifierValue>(identifier->name)));

        // Calculate statements for RHS
        std::vector<std::unique_ptr<Statement>> statements =
            this->evaluateOperations(remainingTokens, identifier->name);

        // Append RHS calculations
        for (size_t i = 0; i < statements.size(); i++) {
          program.addAnyStatement(std::move(statements[i]));
        }
      }

      if (buffer[0]->tokenType == TokenType::RETURN &&
          buffer[1]->tokenType == TokenType::IDENTIFIER) {

        IdentifierToken* identifier = dynamic_cast<IdentifierToken*>(buffer[1]);
        std::unique_ptr<ReturnStatement> returnStatement =
            std::make_unique<ReturnStatement>(
                std::make_unique<IdentifierValue>(identifier->name));

        program.addReturn(std::move(returnStatement));
      }
      buffer.clear();
    }
  }
  std::cout << "program size: " << program.size() << "\n";
  return program;
}