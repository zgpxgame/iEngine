set(PROJECT_NAME plugin_opengl)

set(HEADER_FILES
  include/GLPrerequisites.h
  include/GLRenderSystem.h
  include/GLSupport.h
  include/GLTexture.h
  include/GLTextureManager.h
  include/Win32GLSupport.h
  include/Win32Prerequisites.h
  include/Win32Window.h
)

set(SOURCE_FILES
  src/GLEngineDll.cpp
  src/glew.cpp
  src/GLRenderSystem.cpp
  src/GLSupport.cpp
  src/GLTexture.cpp
  src/GLTextureManager.cpp
  src/Win32GLSupport.cpp
  src/Win32Window.cpp
)

include_directories(include)
include_directories(${iEngine_SOURCE_DIR}/src)
include_directories(${iEngine_SOURCE_DIR}/src/renderer/include)

add_definitions(-DOGRE_GL_USE_MULTITEXTURING)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "plugins")
add_dependencies(${PROJECT_NAME} renderer)
target_link_libraries(${PROJECT_NAME} renderer)
target_link_libraries(${PROJECT_NAME} opengl32.lib glu32.lib)

# …Ë÷√ ‰≥ˆ¬∑æ∂
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)