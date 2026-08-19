#include <string>
#include <vector>

class ProgramText {
private:
  std::vector<std::string> lines;

public:
  ProgramText() : lines() {}

  void addLine(const std::string& line) { lines.push_back(line); };
  const std::string& getLine(size_t i) const { return lines[i]; };
};