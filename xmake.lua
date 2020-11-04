
set_config("buildir", "_build")

target("Crown")
  set_kind("binary")
  set_languages("cxx17")

  add_includedirs(".", "src", "libs")
  add_files("src/*.cpp")
  add_files("src/framework/*.cpp")
  add_files("libs/nanovg/nanovg.c")
  add_includedirs("libs/libxm")
  add_defines("XM_DEFENSIVE=0", "XM_LIBXMIZE_DELTA_SAMPLES=0", "XM_DEBUG=0", "XM_LINEAR_INTERPOLATION=0")
  add_files("libs/libxm/src/*.c|libxmize.c")

  if is_plat("windows") then
    add_files("libs/glad/glad_gl.c")
    add_files("src/platform/platform_glfw.cpp")
  end

  if is_plat("linux") then
    add_syslinks("GL", "glfw", "dl", "pthread")
    add_cxxflags("-no-pie", "-fno-rtti", "-fno-exceptions")
    add_files("src/platform/platform_glfw.cpp")
  end

  if is_plat("web") then
    local emscripten_system = "libs/emscripten/system"

    add_defines(
      "NDEBUG",
      "__WAJIC__",
      "__EMSCRIPTEN__",
      "_LIBCPP_ABI_VERSION=2",
      "TWEAKABLE_DISABLED")
      
    add_includedirs(
      "libs/wajic",
      emscripten_system .. "/include",
      emscripten_system .. "/include/libcxx",
      emscripten_system .. "/include/compat",
      emscripten_system .. "/include/libc",
      emscripten_system .. "/lib/libc/musl/arch/emscripten")
      
    add_files("src/platform/platform_wajic.cpp")
    
    set_toolchains("clang")
    set_arch("wasm32")
    set_filename("output.wasm")   
    set_policy("check.auto_ignore_flags", false)    
    add_cxflags(
      "-target wasm32",
      "-Os", "-flto",
      "-fno-rtti",
      "-fno-exceptions",
      "-fno-threadsafe-statics",
      "-Wall",
      "-Wno-unused-command-line-argument")

    add_ldflags(
      "-target wasm32",
      "-nostartfiles",
      "-nodefaultlibs",
      "-nostdinc",
      "-nostdinc++",
      "-fvisibility=hidden",
      "-Xlinker " .. emscripten_system .. "/system.bc",
      "-Xlinker -strip-all",
      "-Xlinker -gc-sections",
      "-Xlinker -no-entry",
      "-Xlinker -allow-undefined",
      "-Xlinker -export=__wasm_call_ctors",
      "-Xlinker -export=main",
      "-Xlinker -export=malloc",
      "-Xlinker -export=free")

    -- generate JavaScript interface using WAjicUp
    after_link(function (target)
      local flags = {
        "-rle",
      }
      os.run("node %s %s %s %s",
        "libs/wajic/wajicup.js",
        table.concat(flags, " "),
        "$(buildir)/web/wasm32/release/output.wasm",
        "docs/game.js")
    end)
  end

  if is_plat("android") then
    set_kind("shared")
    
    add_defines("ANDROID")
    
    add_files(
      "src/platform/platform_android.cpp",
      "libs/android/android_native_app_glue.c")

    add_includedirs("libs/android")

    add_syslinks("m", "GLESv3", "EGL", "android")

    set_policy("check.auto_ignore_flags", false)
    add_cxflags(
      "-Os",
      "-fno-rtti",
      "-fno-exceptions",
      "-fno-threadsafe-statics",
      "-Wall",
      "-uANativeActivity_onCreate",    
      "-fvisibility=hidden",
      "-ffunction-sections",
      "-fdata-sections",      
      "-Wl,--gc-sections -s")
  end

