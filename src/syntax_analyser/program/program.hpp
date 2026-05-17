#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/primitives/primitive.hpp"
#include "syntax_analyser/statement/primitives/primitive_type.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/value/identifier/identifier.hpp"
#include "syntax_analyser/statement/value/value.hpp"
#include <iostream>
#include <memory>
#include <vector>
class Program {
private:
  std::vector<std::unique_ptr<Statement>> statements;

public:
  Program();

  void addAssignment(std::unique_ptr<AssignmentStatement> statement) {
    this->statements.push_back(std::move(statement));
  }

  void addInitialisation(std::unique_ptr<InitialisationStatement> statement) {
    this->statements.push_back(std::move(statement));
  }

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
          NumberValue statementValue = assignmentStatement->value;

          std::cout << identifier.name << " = " << statementValue.value
                    << ";\n";
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
        }
      }
    }
  }
};