#include "generation/primitives/uint8/uint8_variable.hpp"
#include "generation/type.hpp"

#include <functional>

class Uint8Builder : public BuilderType {
public:
  Uint8Builder() : BuilderType() {};

  std::unique_ptr<Variable> makeVariable(Builder& builder) const override {
    return std::make_unique<Uint8Variable>(builder);
  };
};