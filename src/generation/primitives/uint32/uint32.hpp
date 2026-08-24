#include <functional>

#include "generation/primitives/uint32/uint32_variable.hpp"
#include "generation/type.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"
#include "generation/primitives/builder_type.hpp"

class Uint32Builder : public BuilderType {
public:
  Uint32Builder() : BuilderType((uint8_t)BuilderTypeID::UINT32) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Uint32Variable>(builder, *this,
                                            initialisationStatement);
  };
};