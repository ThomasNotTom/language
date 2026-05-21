#include <regex>
#include <string>

class Matcher {
public:
  static bool isNumber(std::string string) {
    return std::regex_match(string, std::regex("^-?[0-9]+$"));
  };
};