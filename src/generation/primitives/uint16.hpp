#pragma once

#include "generation/builder/builder.hpp"
#include "generation/primitives/uint.hpp"

#include <cstdint>

class BuilderUint16 : public BuilderUintPrimitive {
private:
public:
  BuilderUint16(Builder& builder, std::string name)
      : BuilderUintPrimitive(BuilderUintType::UINT16, builder,
                             builder.getUint16(), name) {};

  void storeValue(uint16_t value) {
    this->builder.store(builder.createConst16(value), this->alloc);
  };

  void addValue(uint16_t value) {
    builder.add(this->alloc, builder.createConst16(value), "add_out");
  };
};