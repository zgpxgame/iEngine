set(PROJECT_NAME plugin_particle)

set(HEADER_FILES           
  include/AreaEmitter.h
  include/BoxEmitter.h
  include/BoxEmitterFactory.h
  include/ColourFaderAffector.h
  include/ColourFaderAffector2.h
  include/ColourFaderAffectorFactory.h
  include/ColourFaderAffectorFactory2.h
  include/CylinderEmitter.h
  include/CylinderEmitterFactory.h
  include/EllipsoidEmitter.h
  include/EllipsoidEmitterFactory.h
  include/HollowEllipsoidEmitter.h
  include/HollowEllipsoidEmitterFactory.h
  include/LinearForceAffector.h
  include/LinearForceAffectorFactory.h
  include/ParticleFXPrerequisites.h
  include/PointEmitter.h
  include/PointEmitterFactory.h
  include/RingEmitter.h
  include/RingEmitterFactory.h
  include/ScaleAffector.h
  include/ScaleAffectorFactory.h
)

set(SOURCE_FILES
  src/AreaEmitter.cpp
  src/BoxEmitter.cpp
  src/ColourFaderAffector.cpp
  src/ColourFaderAffector2.cpp
  src/CylinderEmitter.cpp
  src/EllipsoidEmitter.cpp
  src/HollowEllipsoidEmitter.cpp
  src/LinearForceAffector.cpp
  src/ParticleFX.cpp
  src/PointEmitter.cpp
  src/RingEmitter.cpp
  src/ScaleAffector.cpp
)

include_directories(include)
include_directories(${iEngine_SOURCE_DIR}/src)
include_directories(${iEngine_SOURCE_DIR}/src/renderer/include)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DPLUGIN_ParticleFX_EXPORTS)

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