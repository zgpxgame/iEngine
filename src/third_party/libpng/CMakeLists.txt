set(PROJECT_NAME libpng)

add_library(${PROJECT_NAME} STATIC
  png.c
  pngerror.c
  pngget.c
  pngmem.c
  pngpread.c
  pngread.c
  pngrio.c
  pngrtran.c
  pngrutil.c
  pngset.c
  pngtest.c
  pngtrans.c
  pngwio.c
  pngwrite.c
  pngwtran.c
  pngwutil.c
  png.h
  pngconf.h
  pngdebug.h
  pnginfo.h
  pnglibconf.h
  pngpriv.h
  pngstruct.h
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
include_directories(${THIRD_PARTY_SOURCE_DIR}/zlib)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "third_party")

# …Ë÷√ ‰≥ˆ¬∑æ∂
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)