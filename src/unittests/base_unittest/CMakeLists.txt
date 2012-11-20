set(PROJECT_NAME base_unittest)

include_directories(${iEngine_SOURCE_DIR}/src)
include_directories(${iEngine_SOURCE_DIR}/src/third_party/test/gtest/include)
include_directories(${iEngine_SOURCE_DIR}/src/third_party/test/gmock/include)
add_definitions(-D "UNICODE" -D"_UNICODE")

add_executable(${PROJECT_NAME}
  bind_unittest.cc
  callback_unittest.cc
  run_all_unittests.cc
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "unittests")
add_dependencies(${PROJECT_NAME} base gtest)
target_link_libraries(${PROJECT_NAME} base gtest gmock)

# …Ë÷√ ‰≥ˆ¬∑æ∂
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)