#include <cstdint>

enum class StatementType : uint8_t {
  INITIALISATION = 1,
  ASSIGNMENT = 2,
  ADDITION = 4,
  SUBTRACTION = 5,
  FUNCTION_CALL = 6
};