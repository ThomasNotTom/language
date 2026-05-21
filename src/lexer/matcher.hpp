#include <regex>
#include <string>

class Matcher {
public:
  static bool isInt(std::string string) {
    return std::regex_match(string, std::regex("^-?[0-9]+$"));
  };
};