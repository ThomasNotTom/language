#include <functional>

#include "generation/primitives/builder_type.hpp"
#include "generation/primitives/uint8/uint8_variable.hpp"
#include "generation/type.hpp"

class Uint8Builder : public BuilderType {
public:
  Uint8Builder() : BuilderType((uint8_t)BuilderTypeID::UINT8) {};

  std::unique_ptr<Variable> makeVariable(
      Builder& builder,
      const InitialisationStatement& initialisationStatement) const override {
    return std::make_unique<Uint8Variable>(builder, *this,
                                           initialisationStatement);
  };

  llvm::Value* makeValue(const Builder& builder,
                         const std::string& value) const override {
    return builder.createConst8(StringConverter::toUint8(value));
  };
};