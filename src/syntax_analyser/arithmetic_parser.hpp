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
    // 3 + 4 - 1 + 2; 8 -> 4
    // 3
    // 3 + 4
    // (3 + 4) - 1

    std::queue<std::reference_wrapper<const Token>> outputQueue =
        std::queue<std::reference_wrapper<const Token>>();
    std::stack<std::reference_wrapper<const Token>> operatorStack =
        std::stack<std::reference_wrapper<const Token>>();

    // while there are tokens to be read:
    for (size_t i = 0; i < tokens.size(); i++) {
      const Token& token = tokens[i];
      //     read a token

      //     if the token is:
      //     - a number:
      if (token.tokenType == TokenType::OTHER) {
        outputQueue.push(token);
        //         put it into the output queue
      }
      //     - a function:
      else if (false) {
        //         push it onto the operator stack
      }
      //     - an operator o1:
      else if (ArithmeticParser::isOperator(token)) {
        const Token& o1 = token;
        //         while (
        while (true) {
          //             there is an operator o2 at the top of the operator
          //             stack
          if (operatorStack.empty()) {
            break;
          }
          const Token& o2 = operatorStack.top();
          //             which is not a left parenthesis,
          if (o2.tokenType == TokenType::BRACKET_OPEN) {
            break;
          }

          uint8_t o1Precedence = ArithmeticParser::getTokenPriority(o1);
          uint8_t o2Precedence = ArithmeticParser::getTokenPriority(o2);

          //             and (o2 has greater
          //             precedence than o1 or (o1 and o2 have the same
          //             precedence and o1 is left-associative))
          //         ):
          if (!(o2Precedence >= o1Precedence)) {
            break;
          }

          //             pop o2 from the operator stack into the output queue
          operatorStack.pop();
          outputQueue.push(o2);
        }
        //         push o1 onto the operator stack
        operatorStack.push(o1);
      }
      //     - a ",":
      //         while the operator at the top of the operator stack is not a
      //         left parenthesis:
      //              pop the operator from the operator stack into the output
      //              queue
      //     - a left parenthesis (i.e. "("):
      else if (token.tokenType == TokenType::BRACKET_OPEN) {

        //         push it onto the operator stack
        operatorStack.push(token);
      }
      //     - a right parenthesis (i.e. ")"):
      else if (token.tokenType == TokenType::BRACKET_CLOSE) {
        //         while the operator at the top of the operator stack is not a
        //         left parenthesis:

        std::reference_wrapper<const Token>& topOperatorToken =
            operatorStack.top();

        while (topOperatorToken.get().tokenType != TokenType::BRACKET_OPEN) {
          //             {assert the operator stack is not empty}
          //             /* If the stack runs out without finding a left
          //             parenthesis, then there are mismatched parentheses. */
          //             pop the operator from the operator stack into the
          //             output queue
          outputQueue.push(topOperatorToken);
          operatorStack.pop();

          topOperatorToken = operatorStack.top();
        }
        //         {assert there is a left parenthesis at the top of the
        //         operator stack} pop the left parenthesis from the operator
        //         stack and discard it if there is a function token at the top
        //         of the operator stack, then:
        //             pop the function from the operator stack into the output
        //             queue
      }
    }
    // /* After the while loop, pop the remaining items from the operator
    // stack into the output queue.
    while (!operatorStack.empty()) {
      const Token& operatorToken = operatorStack.top();
      operatorStack.pop();

      outputQueue.push(operatorToken);
    }
    // */ while there are tokens on the operator stack:
    //     /* If the operator token on the top of the stack is a parenthesis,
    //     then there are mismatched parentheses. */ {assert the operator on
    //     top of the stack is not a (left) parenthesis} pop the operator from
    //     the operator stack onto the output queue

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
    return out;
  }
};