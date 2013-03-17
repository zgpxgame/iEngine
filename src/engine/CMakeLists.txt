set(PROJECT_NAME engine)

set(HEADER_FILES
  engine.h
  window_win32.h
)

set(SOURCE_FILES
  engine.cpp
  window_win32.cpp
)

include_directories(${iEngine_SOURCE_DIR}/src)

#add_definitions(-D "UNICODE" -D"_UNICODE")
add_definitions(-D"COMPONENT_BUILD" -D"BASE_IMPLEMENTATION")

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "iEngine")
add_dependencies(${PROJECT_NAME} renderer)
target_link_libraries(${PROJECT_NAME} renderer base)
target_link_libraries(${PROJECT_NAME} opengl32.lib glu32.lib)

# …Ë÷√ ‰≥ˆ¬∑æ∂
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)