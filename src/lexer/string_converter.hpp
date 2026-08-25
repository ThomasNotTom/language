#pragma once

#include <regex>
#include <string>

class StringConverter {
public:
  static bool isInt(const std::string& string) {
    return std::regex_match(string, std::regex("-?[0-9]+"));
  };

  static bool isDouble(const std::string& string) {
    return std::regex_match(string, std::regex("-?[0-9]+\\.[0-9]+"));
  }

  static uint64_t toUnsignedLongLong(const std::string& string) {
    return std::stoull(string);
  }

  static double toDouble(const std::string& string) {
    return std::stod(string);
  }
};