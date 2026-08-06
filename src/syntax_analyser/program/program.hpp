#pragma once

#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/print/print.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/subtraction/subtraction.hpp"
#include <iostream>
#include <memory>
#include <vector>
class Program {
private:
  std::vector<std::unique_ptr<Statement>> statements;

  void printAssignmentStatement(
      const AssignmentStatement assignmentStatement) const {
    std::string identifierName = assignmentStatement.identifier.name;
    std::string statementValue = assignmentStatement.value.name;

    std::cout << identifierName << " = " << statementValue << ";\n";
  };

  void printInitialisationStatement(
      const InitialisationStatement& initialisationStatement) const {
    std::string typeName = initialisationStatement.type.name;
    std::string identifierName = initialisationStatement.identifier.name;

    std::cout << typeName << " " << identifierName << ";\n";
  };

  void printReturnStatement(const ReturnStatement& returnStatement) const {
    std::string identifierName = returnStatement.value.name;

    std::cout << "return " << identifierName << ";\n";
  }

  void
  printAdditionStatement(const AdditionStatement& additionStatement) const {
    std::string lhs = additionStatement.lhs.name;
    std::string rhs = additionStatement.rhs.name;
    std::cout << additionStatement.identifier.name << " = " << lhs << " + "
              << rhs << ";\n";
  }

  void printSubtractionStatement(
      const SubtractionStatement& subtractionStatement) const {
    std::string lhs = subtractionStatement.lhs.name;
    std::string rhs = subtractionStatement.rhs.name;

    std::cout << subtractionStatement.identifier.name << " = " << lhs << " + "
              << rhs << ";\n";
  }

  void printPrintStatement(const PrintStatement& printStatement) const {
    std::string value = printStatement.value.name;

    std::cout << "print(" << value
              << ")"
                 ";\n";
  }

public:
  Program() {};

  void addStatement(std::unique_ptr<Statement> statement) {
    this->statements.push_back(std::move(statement));
  }

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

          this->printAssignmentStatement(assignmentStatement);
          break;
        }

        case StatementType::SUBTRACTION: {

          const SubtractionStatement& subtractionStatement =
              static_cast<const SubtractionStatement&>(statement);

          this->printSubtractionStatement(subtractionStatement);
          break;

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

        case StatementType::PRINT: {
          const PrintStatement& printStatement =
              static_cast<const PrintStatement&>(statement);

          this->printPrintStatement(printStatement);

          break;
        }
      }
    }
  }
};