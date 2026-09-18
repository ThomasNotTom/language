#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "lexer/tokens/operators/addition/addition.hpp"
#include "lexer/tokens/operators/subtraction/subtraction.hpp"
#include "lexer/tokens/other.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"
#include "syntax_analyser/statement/addition/addition.hpp"
#include "syntax_analyser/statement/assignment/assignment.hpp"
#include "syntax_analyser/statement/context/begin_context.hpp"
#include "syntax_analyser/statement/context/end_context.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "syntax_analyser/statement/statement.hpp"
#include "syntax_analyser/statement/subtraction/subtraction.hpp"

class TokenSegment {
private:
  uint8_t priority;
  std::vector<std::reference_wrapper<const Token>> tokens;

public:
  TokenSegment(uint8_t priority) : priority(priority) {};
  uint8_t getPriority() const { return this->priority; }
  void addToken(std::reference_wrapper<const Token> token) {
    this->tokens.push_back(token);
  };

  std::vector<std::reference_wrapper<const Token>> getTokens() {
    return this->tokens;
  };
};

class ArithmeticParser {
private:
  static uint8_t getTokenPriority(std::reference_wrapper<const Token> token) {
    switch (token.get().tokenType) {
      case TokenType::PLUS:
      case TokenType::MINUS: {
        return 2;
      }

      default: {
        throw std::runtime_error("Cannot get priority of token " +
                                 std::to_string(token.get().tokenType));
      }
    }
  };

  static bool isOperator(const Token& token) {
    switch (token.tokenType) {
      case TokenType::PLUS:
      case TokenType::MINUS:
        return true;
      default:
        return false;
    }
  }

public:
  static std::vector<std::unique_ptr<Statement>>
  parse(const std::vector<std::reference_wrapper<const Token>>& tokens,
        const OtherToken& outToken, const OtherToken& outType) {
    std::cout << "Begin passing\n";
    // 3 + 4 - 1 + 2; 8 -> 4
    // 3
    // 3 + 4
    // (3 + 4) - 1

    std::queue<std::reference_wrapper<const Token>> outputQueue =
        std::queue<std::reference_wrapper<const Token>>();
    std::stack<std::reference_wrapper<const Token>> operatorStack =
        std::stack<std::reference_wrapper<const Token>>();

    // Following the algorithm specified
    // (here)[https://en.wikipedia.org/wiki/Shunting_yard_algorithm]
    for (size_t i = 0; i < tokens.size(); i++) {
      const Token& token = tokens[i];
      if (token.tokenType == TokenType::OTHER) {
        outputQueue.push(token);
      }
      //     - a function:
      else if (false) {
      } else if (ArithmeticParser::isOperator(token)) {
        const Token& o1 = token;
        while (true) {

          if (operatorStack.empty()) {
            break;
          }
          const Token& o2 = operatorStack.top();
          if (o2.tokenType == TokenType::BRACKET_OPEN) {
            break;
          }

          uint8_t o1Precedence = ArithmeticParser::getTokenPriority(o1);
          uint8_t o2Precedence = ArithmeticParser::getTokenPriority(o2);

          if (!(o2Precedence >= o1Precedence)) {
            break;
          }

          operatorStack.pop();
          outputQueue.push(o2);
        }
        operatorStack.push(o1);
      } else if (token.tokenType == TokenType::BRACKET_OPEN) {
        operatorStack.push(token);
      } else if (token.tokenType == TokenType::BRACKET_CLOSE) {

        std::reference_wrapper<const Token>& topOperatorToken =
            operatorStack.top();

        while (topOperatorToken.get().tokenType != TokenType::BRACKET_OPEN) {
          outputQueue.push(topOperatorToken);
          operatorStack.pop();

          topOperatorToken = operatorStack.top();
        }
      }
    }

    while (!operatorStack.empty()) {
      const Token& operatorToken = operatorStack.top();
      operatorStack.pop();

      outputQueue.push(operatorToken);
    }

    std::stack<OtherToken> tokenStack;
    std::vector<std::unique_ptr<Statement>> out;

    out.push_back(std::make_unique<BeginContextStatement>());

    size_t tempVariableCount = 1;

    while (!outputQueue.empty()) {
      const Token& next = outputQueue.front().get();
      outputQueue.pop();

      if (next.tokenType == TokenType::OTHER) {
        tokenStack.push(static_cast<const OtherToken&>(next));
        continue;
      }

      if (ArithmeticParser::isOperator(next)) {
        const OtherToken& tokenaB = tokenStack.top();
        tokenStack.pop();

        const OtherToken& tokenaA = tokenStack.top();
        tokenStack.pop();

        switch (next.tokenType) {
          case TokenType::PLUS: {
            const AdditionToken& additionToken =
                static_cast<const AdditionToken&>(next);

            std::string tempName = "temp_" + std::to_string(tempVariableCount);
            const OtherToken tempToken =
                OtherToken(tempName, TokenMetadata(0, 0, 0));

            tempVariableCount += 1;

            out.push_back(
                std::make_unique<InitialisationStatement>(outType, tempToken));

            out.push_back(std::make_unique<AdditionStatement>(
                tempToken, tokenaA, additionToken, tokenaB));

            tokenStack.push(tempToken);
            break;
          }

          case TokenType::MINUS: {
            const SubtractionToken& subtractionToken =
                static_cast<const SubtractionToken&>(next);

            std::string tempName = "temp_" + std::to_string(tempVariableCount);
            const OtherToken tempToken =
                OtherToken(tempName, TokenMetadata(0, 0, 0));

            tempVariableCount += 1;

            out.push_back(
                std::make_unique<InitialisationStatement>(outType, tempToken));

            out.push_back(std::make_unique<SubtractionStatement>(
                tempToken, tokenaA, subtractionToken, tokenaB));

            tokenStack.push(tempToken);

            break;
          }

          default:
            break;
        }
      }
    }

    const OtherToken& finalResult =
        static_cast<const OtherToken&>(tokenStack.top());
    tokenStack.pop();
    out.push_back(std::make_unique<AssignmentStatement>(outToken, finalResult));

    out.push_back(std::make_unique<EndContextStatement>());

    std::cout << "End passing\n";

    return out;
  }
};