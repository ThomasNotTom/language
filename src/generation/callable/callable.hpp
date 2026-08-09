#pragma once

#include "generation/builder/builder.hpp"
class Variable;

class Callable {
public:
  Callable() {};

  virtual void call(Builder& builder, const Variable& out) = 0;
  virtual void call(Builder& builder, int out) = 0;

  virtual ~Callable() = default;
};