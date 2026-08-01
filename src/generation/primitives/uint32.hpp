#pragma once

#include "generation/builder/builder.hpp"
#include "generation/primitives/uint.hpp"

#include <cstdint>

class BuilderUint32 : public BuilderUintPrimitive {
private:
public:
  BuilderUint32(Builder& builder, std::string name)
      : BuilderUintPrimitive(BuilderUintType::UINT32, builder,
                             builder.getUint32(), name) {};

  void storeValue(uint32_t value) {
    this->builder.store(builder.createConst32(value), this->alloc);
  };

  void addValue(uint32_t value) {
    builder.add(this->alloc, builder.createConst32(value), "add_out");
  };
};