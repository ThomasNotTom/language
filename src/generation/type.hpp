#pragma once

#include "generation/builder/builder.hpp"
#include "generation/variable.hpp"

// enum class TypeID : unsigned int { UINT8 = 1, UINT16 };


class BuilderType {
private:
  unsigned int typeID;

public:
  BuilderType(unsigned int typeID) : typeID(typeID) {};

  unsigned int getTypeID() const { return this->typeID; };

  virtual std::unique_ptr<Variable> makeVariable(Builder& builder) const = 0;

  virtual ~BuilderType() = default;
};