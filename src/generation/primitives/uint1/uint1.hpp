#include "generation/primitives/builder_type.hpp"
#include "generation/primitives/uint1/uint1_variable.hpp"
#include "generation/type.hpp"

class Uint1Builder : public BuilderType {
public:
  Uint1Builder() : BuilderType((uint8_t)BuilderTypeID::UINT1) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Uint1Variable>(builder, *this,
                                           initialisationStatement);
  };

  llvm::Value* makeValue(const Builder& builder,
                         const std::string& value) const override {
    return builder.createConst1(StringConverter::toUint1(value));
  };
};