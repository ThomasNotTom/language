#include "./abstract_syntax_tree.hpp"
#include "lexer/tokens/identifier/identifier.hpp"
#include "lexer/tokens/number/number.hpp"
#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/primitives/primitive.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include <memory>

AbstractSyntaxTree::AbstractSyntaxTree(const TokenContainer& tokenContainer)
    : tokenContainer(tokenContainer) {}

Program AbstractSyntaxTree::parse() {
  Program program;

  std::vector<Token*> buffer;

  for (size_t i = 0; i < this->tokenContainer.getCount(); i++) {

    Token* token = this->tokenContainer.get(i);
    buffer.push_back(token);

    if (token->tokenType == TokenType::END_OF_LINE) {
      // uint8 a = 10;
      if (buffer[0]->tokenType == TokenType::PRIMITIVE &&
          buffer[1]->tokenType == TokenType::IDENTIFIER &&
          buffer[2]->tokenType == TokenType::OPERATOR &&
          buffer[3]->tokenType == TokenType::NUMBER) {

        PrimitiveToken* primitive = dynamic_cast<PrimitiveToken*>(buffer[0]);
        IdentifierToken* identifier = dynamic_cast<IdentifierToken*>(buffer[1]);
        NumberToken* number = dynamic_cast<NumberToken*>(buffer[3]);

        OperatorToken* oper = dynamic_cast<OperatorToken*>(buffer[2]);
        if (oper->operatorType == OperatorType::ASSIGNMENT) {
          std::unique_ptr<InitialisationStatement> initialise =
              std::make_unique<InitialisationStatement>(
                  StatementPrimitiveType::UINT8,
                  std::make_unique<IdentifierValue>(identifier->name));

          program.addInitialisation(std::move(initialise));

          std::unique_ptr<AssignmentStatement> assignment =
              std::make_unique<AssignmentStatement>(identifier->name,
                                                    number->value);

          program.addAssignment(std::move(assignment));
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
  return program;
}