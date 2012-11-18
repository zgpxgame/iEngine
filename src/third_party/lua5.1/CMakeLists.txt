set(PROJECT_LUA51_SHARED_LIB lua5.1)
set(PROJECT_LUA51_CONSOLE lua)
set(PROJECT_LUA51_COMPILER luac)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

#####################################################
# lua5.1 动态库
#####################################################
add_library(${PROJECT_LUA51_SHARED_LIB} SHARED
  lapi.h
  lauxlib.h
  lcode.h
  ldebug.h
  ldo.h
  lfunc.h
  lgc.h
  llex.h
  llimits.h
  lmem.h
  lobject.h
  lopcodes.h
  lparser.h
  lstate.h
  lstring.h
  ltable.h
  ltm.h
  lua.h
  luaconf.h
  lualib.h
  lundump.h
  lvm.h
  lzio.h
  lapi.c
  lauxlib.c
  lbaselib.c
  lcode.c
  ldblib.c
  ldebug.c
  ldo.c
  ldump.c
  lfunc.c
  lgc.c
  linit.c
  liolib.c
  llex.c
  lmathlib.c
  lmem.c
  loadlib.c
  lobject.c
  lopcodes.c
  loslib.c
  lparser.c
  lstate.c
  lstring.c
  lstrlib.c
  ltable.c
  ltablib.c
  ltm.c
  lundump.c
  lvm.c
  lzio.c
)

set_target_properties(${PROJECT_LUA51_SHARED_LIB} PROPERTIES FOLDER "third_party/lua5.1")
set_target_properties(${PROJECT_LUA51_SHARED_LIB} PROPERTIES COMPILE_DEFINITIONS "LUA_BUILD_AS_DLL")

# 设置输出路径
set_target_properties(${PROJECT_LUA51_SHARED_LIB} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)


#####################################################
# lua控制台
#####################################################
add_executable(${PROJECT_LUA51_CONSOLE}
  lua.h
  lua.c
)

set_target_properties(${PROJECT_LUA51_CONSOLE} PROPERTIES FOLDER "third_party/lua5.1")
target_link_libraries(${PROJECT_LUA51_CONSOLE} lua5.1)

# 设置输出路径
set_target_properties(${PROJECT_LUA51_CONSOLE} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)


#####################################################
# lua编译器
#####################################################
add_executable(${PROJECT_LUA51_COMPILER}
  lapi.h
  lauxlib.h
  lcode.h
  ldebug.h
  ldo.h
  lfunc.h
  lgc.h
  llex.h
  llimits.h
  lmem.h
  lobject.h
  lopcodes.h
  lparser.h
  lstate.h
  lstring.h
  ltable.h
  ltm.h
  lua.h
  lzio.h
  lapi.c
  lauxlib.c
  lcode.c
  ldebug.c
  ldo.c
  ldump.c
  lfunc.c
  lgc.c
  llex.c
  lmem.c
  lobject.c
  lopcodes.c
  lparser.c
  lstate.c
  lstring.c
  ltable.c
  ltm.c
  luac.c
  lundump.c
  lvm.c
  lzio.c
  print.c
)

set_target_properties(${PROJECT_LUA51_COMPILER} PROPERTIES FOLDER "third_party/lua5.1")

# 设置输出路径
set_target_properties(${PROJECT_LUA51_COMPILER} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)