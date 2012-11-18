set(PROJECT_NAME plugin_filesystem)

set(HEADER_FILES
  include/FileSystem.h
  include/FileSystemFactory.h
)

set(SOURCE_FILES
  src/FileSystem.cpp
  src/FileSystemFactory.cpp
)

include_directories(include)
include_directories(${iEngine_SOURCE_DIR}/src/renderer/include)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "plugins")
add_dependencies(${PROJECT_NAME} renderer)
target_link_libraries(${PROJECT_NAME} renderer)

# …Ë÷√ ‰≥ˆ¬∑æ∂
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)