
#include "generation/primitives/float16/float16_variable.hpp"
#include "generation/type.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Float16Builder : public BuilderType {
public:
  Float16Builder(unsigned int typeID) : BuilderType(typeID) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Float16Variable>(builder, *this,
                                            initialisationStatement);
  };
};