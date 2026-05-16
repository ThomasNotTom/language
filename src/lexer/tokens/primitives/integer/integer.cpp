#pragma once

#include "./integer.hpp"

Integer::Integer(const uint32_t value)
    : Primitive(PrimitiveType::INTEGER), value(value) {}