#pragma once

#include <map>
#include <memory>
#include <string>

#include "generation/callable/callable.hpp"
#include "generation/context_attribute.hpp"
#include "generation/variable.hpp"

class Context {
private:
  std::map<std::string, std::unique_ptr<BuilderType>> types;
  std::map<std::string, std::unique_ptr<Variable>> symbols;
  std::map<std::string, std::unique_ptr<Callable>> callables;

public:
  Context() : types(), symbols(), callables() {};

  std::map<std::string, std::unique_ptr<BuilderType>>& getTypes() {
    return this->types;
  };

  std::map<std::string, std::unique_ptr<Variable>>& getSymbols() {
    return this->symbols;
  };

  std::map<std::string, std::unique_ptr<Callable>>& getCallables() {
    return this->callables;
  };
};