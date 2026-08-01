#pragma once

#include "generation/builder/builder.hpp"
#include "generation/primitives/uint.hpp"

#include <cstdint>

class BuilderUint64 : public BuilderUintPrimitive {
private:
public:
  BuilderUint64(Builder& builder, std::string name)
      : BuilderUintPrimitive(BuilderUintType::UINT64, builder,
                             builder.getUint64(), name) {};

  void storeValue(uint64_t value) {
    this->builder.store(builder.createConst64(value), this->alloc);
  };

  void addValue(uint64_t value) {
    builder.add(this->alloc, builder.createConst64(value), "add_out");
  };
};