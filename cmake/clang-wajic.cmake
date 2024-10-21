
get_filename_component(wajic_dir "${CMAKE_CURRENT_LIST_DIR}/wajic" ABSOLUTE)
get_filename_component(emscripten_system "${CMAKE_CURRENT_LIST_DIR}/emscripten/system" ABSOLUTE)

set(WAJIC true)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR wasm32)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET wasm32)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET wasm32)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .wasm)

add_compile_definitions(
  NDEBUG
  __WAJIC__
  __EMSCRIPTEN__
  _LIBCPP_ABI_VERSION=2
)

include_directories(
  ${wajic_dir}
  ${emscripten_system}/include
  ${emscripten_system}/include/compat
  ${emscripten_system}/lib/libcxx/include/
  ${emscripten_system}/lib/libc/musl/include
  ${emscripten_system}/lib/libc/musl/arch/emscripten
  ${emscripten_system}/lib/libc/musl/arch/generic
)

add_compile_options(
  -target wasm32
  -Os
  -flto
  -fno-rtti
  -fno-exceptions
  -fno-threadsafe-statics
  -Wall
  -Wno-unused-command-line-argument
)

add_link_options(
  -nostartfiles
  -nodefaultlibs
  -nostdinc
  -nostdinc++
  -fvisibility=hidden
  -Xlinker "${emscripten_system}/system.bc"
)

# these did not work in add_link_options
set(link_options
  -Xlinker -strip-all
  -Xlinker -gc-sections
  -Xlinker --no-entry
  -Xlinker -allow-undefined
  -Xlinker -export=__wasm_call_ctors
  -Xlinker -export=main
  -Xlinker -export=malloc
  -Xlinker -export=free
)
list(JOIN link_options " " link_options)
string(APPEND CMAKE_EXE_LINKER_FLAGS " ${link_options}")

function(wajicup project output)
  add_custom_command(TARGET ${project} POST_BUILD
    COMMAND node "${CMAKE_CURRENT_SOURCE_DIR}/cmake/wajic/wajicup.js" -rle "${CMAKE_CURRENT_BINARY_DIR}/${project}.wasm" "${output}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating JavaScript interface using WAjicUp"
  )
endfunction()
