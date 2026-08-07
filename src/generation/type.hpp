#pragma once

#include "generation/builder/builder.hpp"
#include "generation/variable.hpp"

class BuilderType {
private:
public:
  BuilderType() {};

  virtual std::unique_ptr<Variable> makeVariable(Builder& builder) const = 0;
};