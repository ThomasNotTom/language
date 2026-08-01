#include <string>

class StringConverter {
public:
  static unsigned long long toInt(std::string string) {
    return std::stoull(string);
  }
};