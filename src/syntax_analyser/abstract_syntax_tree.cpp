
#include "syntax_analyser/abstract_syntax_tree.hpp"
#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/other.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/print/print.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"

#include <cstddef>
#include <format>
#include <memory>
#include <stdexcept>
#include <vector>

AbstractSyntaxTree::AbstractSyntaxTree(const TokenContainer& tokenContainer)
    : tokenContainer(tokenContainer) {}

std::vector<std::vector<std::reference_wrapper<const Token>>>
AbstractSyntaxTree::splitToLines(const TokenContainer& fullTokens) {
  std::vector<std::reference_wrapper<const Token>> buffer =
      std::vector<std::reference_wrapper<const Token>>();

  std::vector<std::vector<std::reference_wrapper<const Token>>> lines;

  for (int i = 0; i < fullTokens.getCount(); i++) {

    if (fullTokens.view(i).tokenType == TokenType::END_OF_LINE) {
      lines.push_back(buffer);
      buffer.clear();
      continue;
    }

    buffer.push_back(fullTokens.view(i));
  }

  return lines;
};

std::vector<std::unique_ptr<Statement>> AbstractSyntaxTree::leftToRightParse(
    std::vector<std::reference_wrapper<const Token>> tokens) {
  std::vector<std::unique_ptr<Statement>> outStatements;

  const Token& first = tokens[0];

  if (first.tokenType != TokenType::OTHER) {
    throw std::runtime_error("Asigned token must be other");
  }

  const OtherToken& out = static_cast<const OtherToken&>(first);

  for (int i = 1; i < tokens.size(); i += 2) {
    const Token& nextToken = tokens[i].get();

    if (nextToken.tokenType != TokenType::OPERATOR) {
      throw std::runtime_error("Token adjacent to other must be an operator");
    }

    const OperatorToken& operatorToken =
        static_cast<const OperatorToken&>(nextToken);

    const Token& nextNextToken = tokens[i + 1].get();

    if (nextNextToken.tokenType != TokenType::OTHER) {
      throw std::runtime_error("Token adjacent to operator must be an other");
    }

    const OtherToken& otherToken =
        static_cast<const OtherToken&>(nextNextToken);

    switch (operatorToken.operatorType) {
      case ADDITION: {
        outStatements.push_back(std::make_unique<AdditionStatement>(
            OtherStatementValue(out.name), OtherStatementValue(out.name),
            OtherStatementValue(otherToken.name)));
        break;
      }
    }
  }
  // a = b + c + d
  // AKA
  // a = b
  // a = a + c
  // a = a + d

  return outStatements;
}

Program AbstractSyntaxTree::parse() {
  Program program;

  std::vector<std::vector<std::reference_wrapper<const Token>>> lines =
      this->splitToLines(this->tokenContainer);

  for (size_t i = 0; i < lines.size(); i++) {

    std::vector<std::reference_wrapper<const Token>> row = lines[i];

    const Token& token = this->tokenContainer.view(i);

    // add_other ::= {other} | ("+" {add_other})
    // addition_statement ::= {other} {other} "=" {add_other}";"
    //
    // eg: a = b + c;
    if (row.size() >= 3 && row[0].get().tokenType == TokenType::OTHER &&
        row[1].get().tokenType == TokenType::OTHER &&
        row[2].get().tokenType == TokenType::OPERATOR) {

      const OtherToken& type = static_cast<const OtherToken&>(row[0].get());
      const OtherToken& identifier =
          dynamic_cast<const OtherToken&>(row[1].get());
      const OperatorToken& oper =
          dynamic_cast<const OperatorToken&>(row[2].get());

      program.addStatement(std::make_unique<InitialisationStatement>(
          OtherStatementValue(type.name),
          OtherStatementValue(identifier.name)));

      std::vector<std::unique_ptr<Statement>> statements =
          this->leftToRightParse(std::vector(row.begin() + 1, row.end()));

      for (int i = 0; i < statements.size(); i++) {
        program.addStatement(std::move(statements[i]));
      }
    }

    if (row.size() >= 2 && row[0].get().tokenType == TokenType::RETURN &&
        row[1].get().tokenType == TokenType::OTHER) {
      const OtherToken& identifier =
          dynamic_cast<const OtherToken&>(row[1].get());

      program.addStatement(std::make_unique<ReturnStatement>(
          OtherStatementValue(identifier.name)));
    }
  }
  return program;
}