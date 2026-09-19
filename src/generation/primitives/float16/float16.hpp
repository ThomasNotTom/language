
#include "generation/primitives/builder_type.hpp"
#include "generation/primitives/float16/float16_variable.hpp"
#include "generation/type.hpp"
#include "syntax_analyser/statement/initialisation/initialisation.hpp"

class Float16Builder : public BuilderType {
public:
  Float16Builder() : BuilderType((uint8_t)BuilderTypeID::FLOAT16) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Float16Variable>(builder, *this,
                                             initialisationStatement);
  };

  llvm::Value* makeValue(const Builder& builder,
                         const std::string& value) const override {
    return builder.createFloat16(StringConverter::toFloat16(value));
  };
};