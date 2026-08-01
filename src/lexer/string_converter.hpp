#include <string>

class StringConverter {
public:
  static unsigned long long toUnsignedLongLong(std::string string) {
    return std::stoull(string);
  }
};