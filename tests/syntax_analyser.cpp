#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_test_failure_exception.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <csignal>
#include <cstdlib>
#include <memory>

#include "generation/generator.hpp"
#include "io/file_reader.hpp"
#include "lexer/lexer.hpp"
#include "lexer/token_container/token_container.hpp"
#include "lexer/tokens/end_of_line/end_of_line.hpp"
#include "lexer/tokens/operators/addition/addition.hpp"
#include "lexer/tokens/operators/assignment/assignment.hpp"
#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/other.hpp"
#include "lexer/tokens/print/print.hpp"
#include "lexer/tokens/return/return.hpp"
#include "lexer/tokens/token_type.hpp"
#include "syntax_analyser/abstract_syntax_tree.hpp"
#include "syntax_analyser/program/program.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/print/print.hpp"
#include "syntax_analyser/statement/return/return.hpp"
#include "syntax_analyser/statement/statement.hpp"

// ""
TEST_CASE("Empty token container", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 0);
};

// ";"
TEST_CASE("Empty line", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 0);
};

// "uint8 a;"
TEST_CASE("Variable initialisation", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addOther(OtherToken("uint8"));
  tokenContainer.addOther(OtherToken("a"));
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 1);
  REQUIRE(program.view(0).statementType == StatementType::INITIALISATION);
  const InitialisationStatement& statement =
      static_cast<const InitialisationStatement&>(program.view(0));

  REQUIRE(statement.type.name == "uint8");
  REQUIRE(statement.identifier.name == "a");
};

// "uint8 a = 0;"
TEST_CASE("Variable initialisation and assignment", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addOther(OtherToken("uint8"));
  tokenContainer.addOther(OtherToken("a"));
  tokenContainer.addAssignment(AssignmentToken());
  tokenContainer.addOther(OtherToken("0"));
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 2);

  REQUIRE(program.view(0).statementType == StatementType::INITIALISATION);
  const InitialisationStatement& initStatement =
      static_cast<const InitialisationStatement&>(program.view(0));

  REQUIRE(initStatement.type.name == "uint8");
  REQUIRE(initStatement.identifier.name == "a");

  REQUIRE(program.view(1).statementType == StatementType::ASSIGNMENT);
  const AssignmentStatement& assignStatement =
      static_cast<const AssignmentStatement&>(program.view(1));

  REQUIRE(assignStatement.identifier.name == "a");
  REQUIRE(assignStatement.value.name == "0");
};

// "a = 0;"
TEST_CASE("Variable assignment", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addOther(OtherToken("a"));
  tokenContainer.addAssignment(AssignmentToken());
  tokenContainer.addOther(OtherToken("0"));
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 1);

  REQUIRE(program.view(0).statementType == StatementType::ASSIGNMENT);
  const AssignmentStatement& assignStatement =
      static_cast<const AssignmentStatement&>(program.view(0));

  REQUIRE(assignStatement.identifier.name == "a");
  REQUIRE(assignStatement.value.name == "0");
};

// "uint a = 0 + 1;"
TEST_CASE("Variable initialisation and assignment with arithmetic",
          "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addOther(OtherToken("uint8"));
  tokenContainer.addOther(OtherToken("a"));
  tokenContainer.addAssignment(AssignmentToken());
  tokenContainer.addOther(OtherToken("0"));
  tokenContainer.addAddition(AdditionToken());
  tokenContainer.addOther(OtherToken("1"));
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 3);

  REQUIRE(program.view(0).statementType == StatementType::INITIALISATION);
  const InitialisationStatement& initStatement =
      static_cast<const InitialisationStatement&>(program.view(0));

  REQUIRE(program.view(1).statementType == StatementType::ASSIGNMENT);
  const AssignmentStatement& assignStatement =
      static_cast<const AssignmentStatement&>(program.view(1));

  REQUIRE(assignStatement.identifier.name == "a");
  REQUIRE(assignStatement.value.name == "0");

  REQUIRE(program.view(2).statementType == StatementType::ADDITION);
  const AdditionStatement& additionStatement =
      static_cast<const AdditionStatement&>(program.view(2));

  REQUIRE(additionStatement.identifier.name == "a");
  REQUIRE(additionStatement.lhs.name == "a");
  REQUIRE(additionStatement.rhs.name == "1");
};

// "a = 0 + 1;"
TEST_CASE("Variable assignment with arithmetic", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addOther(OtherToken("a"));
  tokenContainer.addAssignment(AssignmentToken());
  tokenContainer.addOther(OtherToken("0"));
  tokenContainer.addAddition(AdditionToken());
  tokenContainer.addOther(OtherToken("1"));
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 2);

  REQUIRE(program.view(0).statementType == StatementType::ASSIGNMENT);
  const AssignmentStatement& assignStatement =
      static_cast<const AssignmentStatement&>(program.view(0));

  REQUIRE(assignStatement.identifier.name == "a");
  REQUIRE(assignStatement.value.name == "0");

  REQUIRE(program.view(1).statementType == StatementType::ADDITION);
  const AdditionStatement& additionStatement =
      static_cast<const AdditionStatement&>(program.view(1));

  REQUIRE(additionStatement.identifier.name == "a");
  REQUIRE(additionStatement.lhs.name == "a");
  REQUIRE(additionStatement.rhs.name == "1");
};

// "print 1;"
TEST_CASE("Print number", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addPrint(PrintToken());
  tokenContainer.addOther(OtherToken("1"));
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 1);

  REQUIRE(program.view(0).statementType == StatementType::PRINT);
  const PrintStatement& printStatement =
      static_cast<const PrintStatement&>(program.view(0));

  REQUIRE(printStatement.value.name == "1");
};

// "return 1;"
TEST_CASE("Return number", "[syntax analyser]") {
  TokenContainer tokenContainer = TokenContainer();
  tokenContainer.addReturn(ReturnToken());
  tokenContainer.addOther(OtherToken("1"));
  tokenContainer.addEndOfLine(EndOfLineToken());

  Program program = AbstractSyntaxTree(tokenContainer).parse();

  REQUIRE(program.size() == 1);

  REQUIRE(program.view(0).statementType == StatementType::RETURN);
  const ReturnStatement& returnStatement =
      static_cast<const ReturnStatement&>(program.view(0));

  REQUIRE(returnStatement.value.name == "1");
};