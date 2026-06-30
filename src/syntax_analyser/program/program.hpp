#pragma once

#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/assignment/identifier/identifier.hpp"
#include "syntax_analyser/statement/assignment/number/number.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/number/number.hpp"
#include "syntax_analyser/statement/value/value.hpp"
#include <iostream>
#include <memory>
#include <vector>
class Program {
private:
  std::vector<std::unique_ptr<Statement>> statements;

  void printAssignmentNumberStatement(
      const AssignmentNumberStatement assignmentNumberStatement) const {
    std::string identifierName = assignmentNumberStatement.identifier.name;
    NumberValue statementValue = assignmentNumberStatement.value;

    std::cout << identifierName << " = " << statementValue.value << ";\n";
  };

  void printAssignmentIdentifierStatement(
      const AssignmentIdentifierStatement assignmentIdentifierStatement) const {
    std::string identifierName = assignmentIdentifierStatement.identifier.name;
    IdentifierValue statementValue = assignmentIdentifierStatement.value;

    std::cout << identifierName << " = " << statementValue.name << ";\n";
  };

  void printInitialisationStatement(
      const InitialisationStatement& initialisationStatement) const {
    switch (initialisationStatement.type) {
      case StatementPrimitiveType::UINT8: {
        std::cout << "UINT8 ";
        break;
      }
    }

    std::cout << initialisationStatement.identifier->name << ";\n";
  };

  void printReturnStatement(const ReturnStatement& returnStatement) const {
    switch (returnStatement.value->statementValueType) {
      case StatementValueType::NUMBER: {
        const NumberValue& numberValue =
            static_cast<const NumberValue&>(*returnStatement.value);
        std::cout << "RETURN " << numberValue.value << ";\n";
        break;
      }

      case StatementValueType::IDENTIFIER: {
        const IdentifierValue& identifierValue =
            static_cast<const IdentifierValue&>(*returnStatement.value);
        std::cout << "RETURN " << identifierValue.name << ";\n";

        break;
      }
    }
  }

  std::string
  getStatementValueString(const StatementValue& statementValue) const {
    std::string out = "";
    if (statementValue.statementValueType == StatementValueType::NUMBER) {
      const NumberValue& numberValue =
          static_cast<const NumberValue&>(statementValue);

      out = std::to_string(numberValue.value);

    } else if (statementValue.statementValueType ==
               StatementValueType::IDENTIFIER) {
      const IdentifierValue& identifierValue =
          static_cast<const IdentifierValue&>(statementValue);
      out = identifierValue.name;
    }

    return out;
  }

  void
  printAdditionStatement(const AdditionStatement& additionStatement) const {
    std::string lhs = this->getStatementValueString(*additionStatement.lhs);
    std::string rhs = this->getStatementValueString(*additionStatement.rhs);

    std::cout << additionStatement.identifier.name << " = " << lhs << " + "
              << rhs << ";\n";
  }

public:
  Program();

  void addStatement(std::unique_ptr<Statement> statement) {
    this->statements.push_back(std::move(statement));
  }

  // void addAssignment(std::unique_ptr<AssignmentStatement> statement) {
  //   this->statements.push_back(std::move(statement));
  // }

  // void addInitialisation(std::unique_ptr<InitialisationStatement> statement)
  // {
  //   this->statements.push_back(std::move(statement));
  // }

  // void addReturn(std::unique_ptr<ReturnStatement> statement) {
  //   this->statements.push_back(std::move(statement));
  // }

  // void addAddition(std::unique_ptr<AdditionStatement> statement) {
  //   this->statements.push_back(std::move(statement));
  // }

  size_t size() const { return this->statements.size(); }

  const Statement& view(size_t i) const { return *this->statements[i]; }

  void print() const {
    std::cout << "-- Program --\n";
    std::cout << "count: " << this->statements.size() << "\n";
    std::cout << "statements: " << "\n";
    for (size_t i = 0; i < this->statements.size(); i++) {
      const Statement& statement = this->view(i);

      switch (statement.statementType) {
        case StatementType::ASSIGNMENT: {
          const AssignmentStatement& assignmentStatement =
              static_cast<const AssignmentStatement&>(statement);

          IdentifierValue identifier = assignmentStatement.identifier;

          switch (assignmentStatement.assignmentType) {
            case AssignmentType::NUMBER: {
              const AssignmentNumberStatement& assignmentNumberStatement =
                  static_cast<const AssignmentNumberStatement&>(
                      assignmentStatement);
              this->printAssignmentNumberStatement(assignmentNumberStatement);
              break;
            }

            case AssignmentType::IDENTIFIER: {
              const AssignmentIdentifierStatement&
                  assignmentIdentifierStatement =
                      static_cast<const AssignmentIdentifierStatement&>(
                          assignmentStatement);

              this->printAssignmentIdentifierStatement(
                  assignmentIdentifierStatement);

              break;
            }
          }
          break;
        }

        case StatementType::INITIALISATION: {
          const InitialisationStatement& initialisationStatement =
              static_cast<const InitialisationStatement&>(statement);

          this->printInitialisationStatement(initialisationStatement);

          break;
        }

        case StatementType::RETURN: {
          const ReturnStatement& returnStatement =
              static_cast<const ReturnStatement&>(statement);

          this->printReturnStatement(returnStatement);

          break;
        }

        case StatementType::ADDITION: {
          const AdditionStatement& additionStatement =
              static_cast<const AdditionStatement&>(statement);

          this->printAdditionStatement(additionStatement);

          break;
        }
      }
    }
  }
};