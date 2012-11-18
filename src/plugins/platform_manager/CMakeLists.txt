set(PROJECT_NAME plugin_platform_manager)

set(HEADER_FILES
  include/resource.h
  include/Win32ConfigDialog.h
  include/Win32ErrorDialog.h
  include/Win32Prerequisites.h
  include/Win32Timer.h
)

set(SOURCE_FILES
  src/Win32ConfigDialog.cpp
  src/Win32ErrorDialog.cpp
  src/Win32PlatformDll.cpp
  src/Win32Timer.cpp
  res/OgreWin32Resources.rc
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