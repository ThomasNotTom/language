#include <functional>

#include "generation/primitives/builder_type.hpp"
#include "generation/primitives/uint16/uint16_variable.hpp"
#include "generation/type.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Uint16Builder : public BuilderType {
public:
  Uint16Builder() : BuilderType((uint8_t)BuilderTypeID::UINT16) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Uint16Variable>(builder, *this,
                                            initialisationStatement);
  };
};