#include <cstdint>

enum class StatementType : uint8_t {
  INITIALISATION = 1,
  ASSIGNMENT = 2,
  ADDITION = 4,
  SUBTRACTION = 5,
  FUNCTION_CALL = 6,
  CONTEXT_BEGIN = 7,
  CONTEXT_END = 8,
  AND = 9,
  OR = 10,
  NOT = 11,
};