
#include "syntax_analyser/abstract_syntax_tree.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "io/program_text.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/bracket/bracket_open.hpp"
#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/operators/operator_type.hpp"
#include "lexer/tokens/other.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"
#include "syntax_analyser/program/program.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/function_call/function_call.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/subtraction/subtraction.hpp"

AbstractSyntaxTree::AbstractSyntaxTree(const TokenContainer& tokenContainer,
                                       const ProgramText& programText)
    : tokenContainer(tokenContainer), programText(programText) {}

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
    std::vector<std::reference_wrapper<const Token>> tokens,
    const OtherToken& outToken) {
  std::vector<std::unique_ptr<Statement>> outStatements;
  for (int i = 1; i < tokens.size(); i += 2) {
    const Token& nextToken = tokens[i].get();
    if (nextToken.tokenType != TokenType::OPERATOR) {
      throw std::runtime_error("Token adjacent to other must be an operator");
    }

    const OperatorToken& operatorToken =
        static_cast<const OperatorToken&>(nextToken);

    const Token& nextNextToken = tokens[i + 1].get();

    if (nextNextToken.tokenType != TokenType::OTHER) {
      std::string out = "\n";
      out += std::to_string(nextToken.metadata.line);
      out += ": ";
      out += this->programText.getLine(nextToken.metadata.line);
      out += "\n";
      out += "Token adjacent to operator (";
      // TODO: Display token character
      out += std::to_string(operatorToken.tokenType);
      out += ") must be an other (";
      out += std::to_string(TokenType::OTHER);
      out += ")";

      throw std::runtime_error(out);
    }

    const OtherToken& otherToken =
        static_cast<const OtherToken&>(nextNextToken);

    switch (operatorToken.operatorType) {
      case ADDITION: {
        outStatements.push_back(std::make_unique<AdditionStatement>(
            outToken, outToken, operatorToken, otherToken));
        break;

        case SUBTRACTION: {
          outStatements.push_back(std::make_unique<SubtractionStatement>(
              outToken, outToken, operatorToken, otherToken));
          break;
        }

        default:

          std::string out = "\n";
          out += std::to_string(otherToken.metadata.line + 1);
          out += ": ";
          out += this->programText.getLine(otherToken.metadata.line);
          out += "\n";
          out += "Operator on right-hand-side of assignment (";
          out += std::to_string(operatorToken.operatorType);
          out += ") must be addition (";
          out += std::to_string(OperatorType::ADDITION);
          out += ") or subtraction (";
          out += std::to_string(OperatorType::SUBTRACTION);
          out += ")";

          throw std::runtime_error(out);
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

    // initialisation ::= {other} {other}";"
    //
    // eg: uint8 a;
    if (row.size() == 2 && row[0].get().tokenType == TokenType::OTHER &&
        row[1].get().tokenType == TokenType::OTHER) {

      const OtherToken& type = static_cast<const OtherToken&>(row[0].get());
      const OtherToken& identifier =
          dynamic_cast<const OtherToken&>(row[1].get());

      program.addStatement(
          std::make_unique<InitialisationStatement>(type, identifier));
      continue;
    }

    // add_other ::= {other} | ("+" {add_other})
    // addition_statement ::= {other} {other} "=" {add_other}";"
    //
    // eg: uint8 a = b + c;
    if (row.size() >= 3 && row[0].get().tokenType == TokenType::OTHER &&
        row[1].get().tokenType == TokenType::OTHER &&
        row[2].get().tokenType == TokenType::OPERATOR) {

      const OtherToken& type = static_cast<const OtherToken&>(row[0].get());
      const OtherToken& identifier =
          static_cast<const OtherToken&>(row[1].get());
      const OperatorToken& oper =
          static_cast<const OperatorToken&>(row[2].get());

      program.addStatement(
          std::make_unique<InitialisationStatement>(type, identifier));

      const OtherToken& value = static_cast<const OtherToken&>(row[3].get());

      program.addStatement(
          std::make_unique<AssignmentStatement>(identifier, value));

      if (row.size() == 4) {
        continue;
      }
      std::vector<std::reference_wrapper<const Token>> remaining =
          std::vector(row.begin() + 3, row.end());

      std::vector<std::unique_ptr<Statement>> statements =
          this->leftToRightParse(remaining, identifier);

      for (int i = 0; i < statements.size(); i++) {
        program.addStatement(std::move(statements[i]));
      }
      continue;
    }

    // add_other ::= {other} | ("+" {add_other})
    // addition_statement ::= {other} "=" {add_other}";"
    //
    // eg: a = b + c;
    if (row.size() >= 2 && row[0].get().tokenType == TokenType::OTHER &&
        row[1].get().tokenType == TokenType::OPERATOR) {
      const OtherToken& identifier =
          dynamic_cast<const OtherToken&>(row[0].get());

      const OperatorToken& oper =
          dynamic_cast<const OperatorToken&>(row[1].get());

      const OtherToken& value = dynamic_cast<const OtherToken&>(row[2].get());
      program.addStatement(
          std::make_unique<AssignmentStatement>(identifier, value));

      if (row.size() == 3) {
        continue;
      }

      std::vector<std::reference_wrapper<const Token>> remaining =
          std::vector(row.begin() + 2, row.end());
      std::vector<std::unique_ptr<Statement>> statements =
          this->leftToRightParse(remaining, identifier);

      for (int i = 0; i < statements.size(); i++) {
        program.addStatement(std::move(statements[i]));
      }
      continue;
    }

    if (row[0].get().tokenType == TokenType::OTHER &&
        row[1].get().tokenType == TokenType::BRACKET_OPEN &&
        row[row.size() - 1].get().tokenType == TokenType::BRACKET_CLOSE) {
      const OtherToken& identifier =
          static_cast<const OtherToken&>(row[0].get());
      const BracketOpen& bracketOpen =
          static_cast<const BracketOpen&>(row[1].get());

      std::vector<OtherToken> parameters;

      for (size_t i = 2; i < row.size() - 1; i++) {
        parameters.push_back(static_cast<const OtherToken&>(row[i].get()));
      }

      program.addStatement(
          std::make_unique<FunctionCallStatement>(identifier, parameters));
    }
  }
  return program;
}