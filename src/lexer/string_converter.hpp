#include <string>

class StringConverter {
public:
  static int toInt(std::string string) { return std::stoi(string); }
};