#include <cstdint>
#include <map>
#include <stack>
#include <vector>

#include "lexer/tokens/operators/operator.hpp"
#include "lexer/tokens/other.hpp"
#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"
#include "syntax_analyser/statement/statement.hpp"

enum class TokenPriority : uint8_t {
  BRACKET = 1,
  ADDITION = 2,
  SUBTRACTION = 3
};

// std::map<OperatorToken> a;

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
public:
  std::stack<TokenSegment>
  parse(const std::vector<std::reference_wrapper<const Token>>& tokens,
        const OtherToken& outToken) {
    // 3 + 4 - 1 + 2; 8 -> 4
    // 3
    // 3 + 4
    // (3 + 4) - 1

    std::stack<std::reference_wrapper<const Token>> inputStack =
        std::stack<std::reference_wrapper<const Token>>();
    std::stack<std::reference_wrapper<const Token>> outputStack =
        std::stack<std::reference_wrapper<const Token>>();
    std::stack<std::reference_wrapper<const Token>> operatorStack =
        std::stack<std::reference_wrapper<const Token>>();

    for (size_t i = 0; i < tokens.size(); i++) {
      inputStack.push(tokens[i]);
    }

    size_t i = 0;

    while (!inputStack.empty()) {
      const Token& next = inputStack.top();
      inputStack.pop();

      if (next.tokenType == TokenType::OTHER) {
        outputStack.push(next);
        i += 1;
        continue;
      }

      if (next.tokenType == TokenType::OPERATOR) {
        const OperatorToken& nextOperator =
            static_cast<const OperatorToken&>(next);
        uint8_t priority;

        if (nextOperator.tokenType ==TokenType::)
          outputStack.push(next);
        i += 1;
        continue;
      }
    };

    outputStack.push(tokens[0]);

    return tokenStack;
  }
};