#pragma once

#include "lexer/token_container/token_container.hpp"
#include "syntax_analyser/program/program.hpp"

class AbstractSyntaxTree {
private:
  const TokenContainer& tokenContainer;

  StatementPrimitiveType
  getStatementPrimitiveTypeFromPrimitiveType(PrimitiveType primitiveType);

  std::vector<std::unique_ptr<Statement>>
  evaluateOperations(std::vector<std::reference_wrapper<const Token>> tokens,
                     std::string outputIdentifier);

  std::unique_ptr<InitialisationStatement>
  makeInitialisationStatement(PrimitiveType primitiveType,
                              std::string identifierName);

public:
  AbstractSyntaxTree(const TokenContainer& tokenContainer);

  Program parse();
};