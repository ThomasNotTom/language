#pragma once

#include "./float.hpp"

Float::Float(const float value)
    : Primitive(PrimitiveType::FLOAT), value(value) {}