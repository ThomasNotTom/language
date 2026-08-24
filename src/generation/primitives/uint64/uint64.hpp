#include <functional>

#include "generation/primitives/builder_type.hpp"
#include "generation/primitives/uint64/uint64_variable.hpp"
#include "generation/type.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Uint64Builder : public BuilderType {
public:
  Uint64Builder() : BuilderType((uint8_t)BuilderTypeID::UINT64) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Uint64Variable>(builder, *this,
                                            initialisationStatement);
  };
};