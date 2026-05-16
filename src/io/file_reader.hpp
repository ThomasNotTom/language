#include <cstdint>
#include <fstream>
#include <string>

class FileReader {
private:
  const std::string filepath;

  std::ifstream inputStream;

  uint32_t readIndex = 0;

public:
  FileReader(const std::string filepath);

  char readCharacter();
  void increment();
};