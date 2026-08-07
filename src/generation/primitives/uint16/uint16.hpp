#include "generation/primitives/uint16/uint16_variable.hpp"
#include "generation/type.hpp"

#include <functional>

class Uint16Builder : public BuilderType {
public:
  Uint16Builder() : BuilderType() {};

  std::unique_ptr<Variable> makeVariable(Builder& builder) const override {
    return std::make_unique<Uint16Variable>(builder);
  };
};