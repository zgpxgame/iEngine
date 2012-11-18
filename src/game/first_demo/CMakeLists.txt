set(PROJECT_NAME demo)

add_executable(${PROJECT_NAME} WIN32
  ExampleApplication.h
  ExampleFrameListener.h
  demo_main.cpp
)

include_directories(${iEngine_SOURCE_DIR}/src/renderer/include)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "game")
add_dependencies(${PROJECT_NAME} renderer)
target_link_libraries(${PROJECT_NAME} renderer)

# …Ë÷√ ‰≥ˆ¬∑æ∂
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)