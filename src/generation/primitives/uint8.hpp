#pragma once

#include "generation/builder/builder.hpp"
#include "generation/primitives/uint.hpp"

#include <cstdint>

class BuilderUint8 : public BuilderUintPrimitive {
private:
public:
  BuilderUint8(Builder& builder, std::string name)
      : BuilderUintPrimitive(BuilderUintType::UINT8, builder,
                             builder.getUint8(), name) {};

  void storeValue(uint8_t value) {
    this->builder.store(builder.createConst8(value), this->alloc);
  };

  void addValue(uint8_t value) {
    builder.add(this->alloc, builder.createConst8(value), "add_out");
  };
};