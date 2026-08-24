
#include "generation/primitives/float32/float32_variable.hpp"
#include "generation/type.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

#include "generation/primitives/builder_type.hpp"

class Float32Builder : public BuilderType {
public:
  Float32Builder() : BuilderType((uint8_t)BuilderTypeID::FLOAT32) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Float32Variable>(builder, *this,
                                             initialisationStatement);
  };
};