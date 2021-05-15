// Copyright ii887522

#ifdef TEST

#define ALLOCATOR_IMPLEMENTATIONS
#include <Any/allocator.h>

#include "../Any/CommandLine.test.h"
#include "util.test.h"

namespace ii887522::texturePacker {

static int main() {
  testCommandLine();
  testUtil();
  return 0;
}

}  // namespace ii887522::texturePacker

int main() {
  return ii887522::texturePacker::main();
}

#endif
