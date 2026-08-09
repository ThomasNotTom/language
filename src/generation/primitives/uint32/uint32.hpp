#include "generation/primitives/uint32/uint32_variable.hpp"
#include "generation/type.hpp"

#include <functional>

class Uint32Builder : public BuilderType {
public:
  Uint32Builder(unsigned int typeID) : BuilderType(typeID) {};

  std::unique_ptr<Variable> makeVariable(Builder& builder) const override {
    return std::make_unique<Uint32Variable>(builder, *this);
  };
};