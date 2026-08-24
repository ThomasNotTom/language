
#include <cstdint>

#include "generation/primitives/builder_type.hpp"
#include "generation/primitives/float64/float64_variable.hpp"
#include "generation/type.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Float64Builder : public BuilderType {
public:
  Float64Builder() : BuilderType((uint8_t)BuilderTypeID::FLOAT64) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Float64Variable>(builder, *this,
                                             initialisationStatement);
  };
};