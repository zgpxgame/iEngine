set(PROJECT_ILU ilu)

add_library(${PROJECT_ILU} STATIC 
  ${DEVIL_SOURCE_DIR}/include/IL/devil_internal_exports.h
  ${DEVIL_SOURCE_DIR}/include/IL/ilu.h
  include/ilu_error/ilu_err-arabic.h
  include/ilu_error/ilu_err-dutch.h
  include/ilu_error/ilu_err-english.h
  include/ilu_error/ilu_err-french.h
  include/ilu_error/ilu_err-german.h
  include/ilu_error/ilu_err-japanese.h
  include/ilu_error/ilu_err-spanish.h
  include/ilu_filter.h
  include/ilu_internal.h
  include/ilu_region.h
  include/ilu_states.h
  src/ilu_error.c
  src/ilu_filter.c
  src/ilu_filter_rcg.c
  src/ilu_internal.c
  src/ilu_main.c
  src/ilu_manip.c
  src/ilu_mipmap.c
  src/ilu_noise.c
  src/ilu_region.c
  src/ilu_rotate.c
  src/ilu_scale.c
  src/ilu_scale2d.c
  src/ilu_scale3d.c
  src/ilu_states.c
  src/ilu_utilities.c
)

include_directories(include)
include_directories(${DEVIL_SOURCE_DIR}/include)
include_directories(${THIRD_PARTY_SOURCE_DIR}/zlib)
include_directories(${THIRD_PARTY_SOURCE_DIR}/libjpeg)
include_directories(${THIRD_PARTY_SOURCE_DIR}/libpng)

set_target_properties(${PROJECT_ILU} PROPERTIES FOLDER "third_party/devil")

# …Ë÷√ ‰≥ˆ¬∑æ∂
set_target_properties(${PROJECT_ILU} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)
