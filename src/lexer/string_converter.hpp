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

  static bool toUint1(const std::string& string) {
    return std::stoull(string) % 2;
  };

  static uint8_t toUint8(const std::string& string) {
    return std::stoull(string);
  }

  static uint16_t toUint16(const std::string& string) {
    return std::stoull(string);
  }

  static uint32_t toUint32(const std::string& string) {
    return std::stoull(string);
  }

  static uint64_t toUint64(const std::string& string) {
    return std::stoull(string);
  }

  static float toFloat16(const std::string& string) {
    return static_cast<float>(std::stod(string));
  }

  static float toFloat32(const std::string& string) {
    return static_cast<float>(std::stod(string));
  }

  static double toFloat64(const std::string& string) {
    return std::stod(string);
  }
};