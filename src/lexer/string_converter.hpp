#pragma once

#include <string>

class StringConverter {
public:
  static uint64_t toUnsignedLongLong(const std::string& string) {
    return std::stoull(string);
  }
};