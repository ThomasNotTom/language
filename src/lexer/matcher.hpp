#pragma once

#include <regex>
#include <string>

class Matcher {
public:
  static bool isInt(const std::string& string) {
    return std::regex_match(string, std::regex("-?[0-9]+"));
  };

  static bool isFloat(const std::string& string) {
    return std::regex_match(string, std::regex("-?[0-9]+\\.[0-9]+"));
  }
};