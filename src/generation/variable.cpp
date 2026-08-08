#include "generation/variable.hpp"
#include "generation/type.hpp"

unsigned int Variable::getType() const {
  return this->builderType.getTypeID();
};
