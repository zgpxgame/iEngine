set(PROJECT_NAME base)

set(HEADER_FILES
  atomicops.h
  atomicops_internals_x86_msvc.h
  atomic_ref_count.h
  at_exit.h
  base_export.h
  base_switches.h
  basictypes.h
  bind.h
  bind_helpers.h
  bind_internal.h
  bind_internal_win.h
  build_config.h
  callback.h
  callback_internal.h
  command_line.h
  compiler_specific.h
  cpu.h
  debug/alias.h
  debug/debugger.h
  debug/stack_trace.h
  eintr_wrapper.h
  file_descriptor_shuffle.h
  file_path.h
  hash_tables.h
  lazy_instance.h
  logging.h
  memory/linked_ptr.h
  memory/mru_cache.h
  memory/raw_scoped_refptr_mismatch_checker.h
  memory/ref_counted.h
  memory/ref_counted_memory.h
  memory/scoped_callback_factory.h
  memory/scoped_handle.h
  memory/scoped_open_process.h
  memory/scoped_ptr.h
  memory/scoped_vector.h
  memory/singleton.h
  memory/weak_ptr.h
  pickle.h
  port.h
  process.h
  process_util.h
  string16.h
  stringprintf.h
  string_number_conversions.h
  string_piece.h
  string_split.h
  string_tokenizer.h
  string_util.h
  string_util_win.h
  synchronization/lock.h
  synchronization/lock_impl.h
  sys_info.h
  sys_string_conversions.h
  task.h
  template_util.h
  third_party/dmg_fp/dmg_fp.h
  third_party/dynamic_annotations/dynamic_annotations.h
  third_party/icu/icu_utf.h
  threading/platform_thread.h
  threading/thread_checker.h
  threading/thread_checker_impl.h
  threading/thread_collision_warner.h
  threading/thread_local.h
  threading/thread_restrictions.h
  time.h
  tuple.h
  utf_string_conversions.h
  utf_string_conversion_utils.h
  vlog.h
  win/scoped_handle.h
  win/windows_version.h
)

set(SOURCE_FILES
  at_exit.cc
  base_switches.cc
  callback_internal.cc
  command_line.cc
  cpu.cc
  debug/alias.cc
  debug/debugger.cc
  debug/debugger_win.cc
  debug/stack_trace.cc
  debug/stack_trace_win.cc
  file_path.cc
  lazy_instance.cc
  logging.cc
  memory/ref_counted.cc
  memory/ref_counted_memory.cc
  memory/weak_ptr.cc
  pickle.cc
  process_util.cc
  process_util_win.cc
  process_win.cc
  stringprintf.cc
  string_number_conversions.cc
  string_piece.cc
  string_split.cc
  string_util.cc
  synchronization/lock.cc
  synchronization/lock_impl_win.cc
  sys_info_win.cc
  sys_string_conversions_win.cc
  task.cc
  third_party/dmg_fp/dtoa_wrapper.cc
  third_party/dmg_fp/g_fmt.cc
  third_party/dynamic_annotations/dynamic_annotations.c
  third_party/icu/icu_utf.cc
  threading/platform_thread_win.cc
  threading/thread_checker_impl.cc
  threading/thread_collision_warner.cc
  threading/thread_local_win.cc
  threading/thread_restrictions.cc
  time.cc
  time_win.cc
  utf_string_conversions.cc
  utf_string_conversion_utils.cc
  vlog.cc
  win/windows_version.cc
)

SOURCE_GROUP("debug" FILES
  debug/alias.h
  debug/debugger.h
  debug/stack_trace.h
  debug/alias.cc
  debug/debugger.cc
  debug/debugger_win.cc
  debug/stack_trace.cc
  debug/stack_trace_win.cc
)

SOURCE_GROUP("memory" FILES
  memory/linked_ptr.h
  memory/mru_cache.h
  memory/raw_scoped_refptr_mismatch_checker.h
  memory/ref_counted.h
  memory/ref_counted_memory.h
  memory/scoped_callback_factory.h
  memory/scoped_handle.h
  memory/scoped_open_process.h
  memory/scoped_ptr.h
  memory/scoped_vector.h
  memory/singleton.h
  memory/weak_ptr.h
  memory/ref_counted.cc
  memory/ref_counted_memory.cc
  memory/weak_ptr.cc
)

SOURCE_GROUP("synchronization" FILES
  synchronization/lock.h
  synchronization/lock_impl.h
  synchronization/lock.cc
  synchronization/lock_impl_win.cc
)

SOURCE_GROUP("third_party\\dmg_fp" FILES
  third_party/dmg_fp/dmg_fp.h
  third_party/dmg_fp/dtoa_wrapper.cc
  third_party/dmg_fp/g_fmt.cc
)

SOURCE_GROUP("third_party\\dynamic_annotations" FILES
  third_party/dynamic_annotations/dynamic_annotations.h
  third_party/dynamic_annotations/dynamic_annotations.c
)

SOURCE_GROUP("third_party\\icu" FILES
  third_party/icu/icu_utf.h
  third_party/icu/icu_utf.cc
)

SOURCE_GROUP("threading" FILES
  threading/platform_thread.h
  threading/thread_checker.h
  threading/thread_checker_impl.h
  threading/thread_collision_warner.h
  threading/thread_local.h
  threading/thread_restrictions.h
  threading/platform_thread_win.cc
  threading/thread_checker_impl.cc
  threading/thread_collision_warner.cc
  threading/thread_local_win.cc
  threading/thread_restrictions.cc
)

SOURCE_GROUP("win" FILES
  win/scoped_handle.h
  win/windows_version.h
  win/windows_version.cc
)

SOURCE_GROUP("misc" FILES
  atomicops.h
  atomicops_internals_x86_msvc.h
  atomic_ref_count.h
  at_exit.h
  base_export.h
  base_switches.h
  basictypes.h
  bind.h
  bind_helpers.h
  bind_internal.h
  bind_internal_win.h
  build_config.h
  callback.h
  callback_internal.h
  command_line.h
  compiler_specific.h
  cpu.h
  eintr_wrapper.h
  file_descriptor_shuffle.h
  file_path.h
  hash_tables.h
  lazy_instance.h
  logging.h
  pickle.h
  port.h
  process.h
  process_util.h
  string16.h
  stringprintf.h
  string_number_conversions.h
  string_piece.h
  string_split.h
  string_tokenizer.h
  string_util.h
  string_util_win.h
  sys_info.h
  sys_string_conversions.h
  task.h
  template_util.h
  time.h
  tuple.h
  utf_string_conversions.h
  utf_string_conversion_utils.h
  vlog.h
  at_exit.cc
  base_switches.cc
  callback_internal.cc
  command_line.cc
  cpu.cc
  file_path.cc
  lazy_instance.cc
  logging.cc
  pickle.cc
  process_util.cc
  process_util_win.cc
  process_win.cc
  stringprintf.cc
  string_number_conversions.cc
  string_piece.cc
  string_split.cc
  string_util.cc
  sys_info_win.cc
  sys_string_conversions_win.cc
  task.cc
  time.cc
  time_win.cc
  utf_string_conversions.cc
  utf_string_conversion_utils.cc
  vlog.cc
)

get_property(PROJECT_PARENT_DIR DIRECTORY PROPERTY PARENT_DIRECTORY)
include_directories(${PROJECT_PARENT_DIR})

add_definitions(-D "UNICODE" -D"_UNICODE")
add_definitions(-D "COMPONENT_BUILD")
add_definitions(-D "BASE_IMPLEMENTATION")
add_definitions(-D "NOMINMAX")
add_definitions(-D "UNIT_TEST")
add_definitions(-D "DYNAMIC_ANNOTATIONS_ENABLED=1")
add_definitions(/wd4251)

# 添加动态库工程
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

# 设置工程属性
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "iEngine")
target_link_libraries(${PROJECT_NAME} dbghelp.lib psapi.lib)

# 设置输出路径
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${iEngine_BINARY_DIR}/bin
)