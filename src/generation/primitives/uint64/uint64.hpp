#include "generation/primitives/uint64/uint64_variable.hpp"
#include "generation/type.hpp"

#include <functional>

class Uint64Builder : public BuilderType {
public:
  Uint64Builder(unsigned int typeID) : BuilderType(typeID) {};

  std::unique_ptr<Variable> makeVariable(Builder& builder) const override {
    return std::make_unique<Uint64Variable>(builder, *this);
  };
};