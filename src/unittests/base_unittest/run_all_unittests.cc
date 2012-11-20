
#include "base/process_util.h"
#include "base/command_line.h"
#include "third_party/test/gtest/include/gtest/gtest.h"

int main(int argc, char **argv) {
#if defined(OS_WIN)
  testing::GTEST_FLAG(catch_exceptions) = false;
#endif
  base::EnableTerminationOnHeapCorruption();
  CommandLine::Init(argc, argv);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
