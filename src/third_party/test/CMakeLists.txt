set(PROJECT_GTEST gtest) # 1.6.0
set(PROJECT_GMOCK gmock) # 1.6.0

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
include_directories(gtest)
include_directories(gmock)
include_directories(gtest/include)
include_directories(gmock/include)

#####################################################
# gtest
#####################################################
set(GROUP_GTEST_INTERNAL
  gtest/include/gtest/internal/gtest-death-test-internal.h
  gtest/include/gtest/internal/gtest-filepath.h
  gtest/include/gtest/internal/gtest-internal.h
  gtest/include/gtest/internal/gtest-linked_ptr.h
  gtest/include/gtest/internal/gtest-param-util-generated.h
  gtest/include/gtest/internal/gtest-param-util.h
  gtest/include/gtest/internal/gtest-port.h
  gtest/include/gtest/internal/gtest-string.h
  gtest/include/gtest/internal/gtest-tuple.h
  gtest/include/gtest/internal/gtest-type-util.h
)

set(GROUP_GTEST_INCLUDE
  gtest/include/gtest/gtest-death-test.h
  gtest/include/gtest/gtest-message.h
  gtest/include/gtest/gtest-param-test.h
  gtest/include/gtest/gtest-printers.h
  gtest/include/gtest/gtest-spi.h
  gtest/include/gtest/gtest-test-part.h
  gtest/include/gtest/gtest-typed-test.h
  gtest/include/gtest/gtest.h
  gtest/include/gtest/gtest_pred_impl.h
  gtest/include/gtest/gtest_prod.h
)

set(GROUP_GTEST_SRC
  gtest/src/gtest-death-test.cc
  gtest/src/gtest-filepath.cc
  gtest/src/gtest-internal-inl.h
  gtest/src/gtest-port.cc
  gtest/src/gtest-printers.cc
  gtest/src/gtest-test-part.cc
  gtest/src/gtest-typed-test.cc
  gtest/src/gtest.cc
)

SOURCE_GROUP("gtest\\include\\gtest\\internal" FILES
  ${GROUP_GTEST_INTERNAL}
)

SOURCE_GROUP("gtest\\include\\gtest" FILES
  ${GROUP_GTEST_INCLUDE}
)

SOURCE_GROUP("gtest\\src" FILES
  ${GROUP_GTEST_SRC}
)

add_library(${PROJECT_GTEST} STATIC
  ${GROUP_GTEST_INTERNAL}
  ${GROUP_GTEST_INCLUDE}
  ${GROUP_GTEST_SRC}
)

set_target_properties(${PROJECT_GTEST} PROPERTIES FOLDER "third_party/test")
# 设置输出路径
set_target_properties(${PROJECT_GTEST} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)

#####################################################
# gmock
#####################################################
set(GROUP_GMOCK_INTERNAL
  gmock/include/gmock/internal/gmock-generated-internal-utils.h
  gmock/include/gmock/internal/gmock-internal-utils.h
  gmock/include/gmock/internal/gmock-port.h
)

set(GROUP_GMOCK_INCLUDE
  gmock/include/gmock/gmock-actions.h
  gmock/include/gmock/gmock-cardinalities.h
  gmock/include/gmock/gmock-generated-actions.h
  gmock/include/gmock/gmock-generated-function-mockers.h
  gmock/include/gmock/gmock-generated-matchers.h
  gmock/include/gmock/gmock-generated-nice-strict.h
  gmock/include/gmock/gmock-more-actions.h
  gmock/include/gmock/gmock-matchers.h
  gmock/include/gmock/gmock-spec-builders.h
  gmock/include/gmock/gmock.h
)

set(GROUP_GMOCK_SRC
  gmock/src/gmock-cardinalities.cc
  gmock/src/gmock-internal-utils.cc
  gmock/src/gmock-matchers.cc
  gmock/src/gmock-spec-builders.cc
  gmock/src/gmock.cc
)

SOURCE_GROUP("gmock\\include\\gmock\\internal" FILES
  ${GROUP_GMOCK_INTERNAL}
)

SOURCE_GROUP("gmock\\include\\gmock" FILES
  ${GROUP_GMOCK_INCLUDE}
)

SOURCE_GROUP("gmock\\src" FILES
  ${GROUP_GMOCK_SRC}
)

add_library(${PROJECT_GMOCK} STATIC
  ${GROUP_GMOCK_INTERNAL}
  ${GROUP_GMOCK_INCLUDE}
  ${GROUP_GMOCK_SRC}
)

set_target_properties(${PROJECT_GMOCK} PROPERTIES FOLDER "third_party/test")
# 设置输出路径
set_target_properties(${PROJECT_GMOCK} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)