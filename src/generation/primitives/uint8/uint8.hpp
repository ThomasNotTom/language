#include <functional>

#include "generation/primitives/uint8/uint8_variable.hpp"
#include "generation/type.hpp"

class Uint8Builder : public BuilderType {
public:
  Uint8Builder(unsigned int typeID) : BuilderType(typeID) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Uint8Variable>(builder, *this,
                                           initialisationStatement);
  };
};