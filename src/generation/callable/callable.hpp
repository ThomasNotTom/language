#pragma once

#include <cstdint>
#include <memory>

#include "generation/builder/builder.hpp"

class Variable;

enum class ParamaterType : uint8_t { VALUE = 1, VARIABLE = 2 };
class Parameter {
private:
  ParamaterType type;

public:
  Parameter(const ParamaterType& type) : type(type) {};

  const ParamaterType& getType() const { return this->type; }
};

class ParameterValue : public Parameter {
private:
  std::string value;

public:
  ParameterValue(const std::string& value)
      : Parameter(ParamaterType::VALUE), value(value) {};

  const std::string& getValue() const { return this->value; }
};

class ParameterVariable : public Parameter {
private:
  const Variable& variable;

public:
  ParameterVariable(const Variable& variable)
      : Parameter(ParamaterType::VARIABLE), variable(variable) {};

  const Variable& getVariable() const { return this->variable; }
};

class Callable {
public:
  Callable() {};

  virtual void
  call(Builder& builder,
       const std::vector<std::unique_ptr<Parameter>>& parameters) = 0;

  virtual ~Callable() = default;
};