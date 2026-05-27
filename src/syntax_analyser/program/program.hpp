#pragma once

#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/assignment/identifier/identifier.hpp"
#include "syntax_analyser/statement/assignment/number/number.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include <iostream>
#include <memory>
#include <vector>
class Program {
private:
  std::vector<std::unique_ptr<Statement>> statements;

public:
  Program();

  void addAnyStatement(std::unique_ptr<Statement> statement) {
    this->statements.push_back(std::move(statement));
  }

  void addAssignment(std::unique_ptr<AssignmentStatement> statement) {
    this->statements.push_back(std::move(statement));
  }

  void addInitialisation(std::unique_ptr<InitialisationStatement> statement) {
    this->statements.push_back(std::move(statement));
  }

  void addReturn(std::unique_ptr<ReturnStatement> statement) {
    this->statements.push_back(std::move(statement));
  }

  size_t size() const { return this->statements.size(); }
  const Statement& get(size_t i) const { return *this->statements[i].get(); }

  void print() const {
    std::cout << "-- Program --\n";
    std::cout << "count: " << this->statements.size() << "\n";
    std::cout << "statements: " << "\n";
    for (size_t i = 0; i < this->statements.size(); i++) {
      Statement* statement = this->statements[i].get();
      switch (statement->statementType) {
        case StatementType::ASSIGNMENT: {
          AssignmentStatement* assignmentStatement =
              (AssignmentStatement*)statement;
          IdentifierValue identifier = assignmentStatement->identifier;

          switch (assignmentStatement->assignmentType) {
            case AssignmentType::NUMBER: {
              AssignmentNumberStatement* assignmentNumberStatement =
                  (AssignmentNumberStatement*)assignmentStatement;

              NumberValue statementValue = assignmentNumberStatement->value;

              std::cout << identifier.name << " = " << statementValue.value
                        << ";\n";
              break;
            }

            case AssignmentType::IDENTIFIER: {
              AssignmentIdentifierStatement* assignmentIdentifierStatement =
                  (AssignmentIdentifierStatement*)assignmentStatement;

              IdentifierValue statementValue =
                  assignmentIdentifierStatement->value;

              std::cout << identifier.name << " = " << statementValue.name
                        << ";\n";
              break;
            }
          }
          break;
        }

        case StatementType::INITIALISATION: {
          InitialisationStatement* initialisationStatement =
              (InitialisationStatement*)statement;

          switch (initialisationStatement->type) {
            case StatementPrimitiveType::UINT8: {
              std::cout << "UINT8 ";
              break;
            }
          }

          std::cout << initialisationStatement->identifier->name << ";\n";
          break;
        }

        case StatementType::RETURN: {
          ReturnStatement* returnStatement = (ReturnStatement*)statement;

          std::cout << "RETURN " << returnStatement->identifier->name << ";\n";
          break;
        }
      }
    }
  }
};