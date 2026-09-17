#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "generation/callable/callable.hpp"
#include "generation/context.hpp"
#include "generation/type.hpp"
#include "generation/variable.hpp"

class ContextContainer {
private:
  std::vector<Context> contexts;

public:
  ContextContainer() : contexts() {};

  void makeEmptyContext() { this->contexts.push_back(Context()); }
  void removeTopContext() { this->contexts.pop_back(); }

  void addSymbol(const std::string& key, std::unique_ptr<Variable> value) {
    this->contexts.back().getSymbols().emplace(key, std::move(value));
  }

  void addType(const std::string& key, std::unique_ptr<BuilderType> value) {
    this->contexts.back().getTypes().emplace(key, std::move(value));
  }

  void addCallable(const std::string& key, std::unique_ptr<Callable> value) {
    this->contexts.back().getCallables().emplace(key, std::move(value));
  }

  // TODO: Reduce replicated code
  std::map<std::string, Variable*> getSymbols() {
    std::map<std::string, Variable*> out;
    for (auto& context : contexts) {
      for (auto& [key, value] : context.getSymbols()) {
        out[key] = value.get();
      }
    }
    return out;
  }

  std::map<std::string, BuilderType*> getTypes() {
    std::map<std::string, BuilderType*> out;
    for (auto& context : contexts) {
      for (auto& [key, value] : context.getTypes()) {
        out[key] = value.get();
      }
    }
    return out;
  }

  std::map<std::string, Callable*> getCallables() {
    std::map<std::string, Callable*> out;
    for (auto& context : contexts) {
      for (auto& [key, value] : context.getCallables()) {
        out[key] = value.get();
      }
    }
    return out;
  }
};