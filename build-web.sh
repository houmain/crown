#!/usr/bin/bash
set -e -u

project="game"

out_dir=$(realpath "web")
wajic_dir="libs/wajic"
emscripten_system="libs/emscripten/system"

clang \
  -isystem${emscripten_system}/include -isystem${emscripten_system}/include/libcxx -isystem${emscripten_system}/include/compat -isystem${emscripten_system}/include/libc -isystem${emscripten_system}/lib/libc/musl/arch/emscripten \
  -Xlinker ${emscripten_system}/system.bc \
  -D__EMSCRIPTEN__ -D_LIBCPP_ABI_VERSION=2 -DNDEBUG -D__WAJIC__ \
  -I${wajic_dir} -I. -Isrc -Ilibs \
  -Os -flto -target wasm32 -nostartfiles -nodefaultlibs -nostdinc -nostdinc++ \
  -Wno-unused-command-line-argument \
  -fno-rtti -fno-exceptions -fno-threadsafe-statics \
  -fvisibility=hidden -Xlinker -strip-all -Xlinker -gc-sections -Xlinker -no-entry -Xlinker -allow-undefined \
  -Xlinker -export=__wasm_call_ctors -Xlinker -export=main -Xlinker -export=malloc \
  libs/nanovg/nanovg.c \
  libs/choreograph/Cue.cpp \
  libs/choreograph/Timeline.cpp \
  libs/choreograph/TimelineItem.cpp \
  -Ilibs/libxm -DXM_DEFENSIVE=0 -DXM_LIBXMIZE_DELTA_SAMPLES=0 -DXM_DEBUG=0 -DXM_LINEAR_INTERPOLATION=0 \
  libs/libxm/src/context.c \
  libs/libxm/src/load.c \
  libs/libxm/src/play.c \
  libs/libxm/src/xm.c \
  src/platform/platform_wajic.cpp \
  src/framework/get_resource.cpp \
  src/framework/ImageFile.cpp \
  src/framework/Target.cpp \
  src/framework/ShaderProgram.cpp \
  src/framework/SpriteAtlas.cpp \
  src/framework/SpriteBatch.cpp \
  src/framework/Texture.cpp \
  src/main.cpp \
  src/Game.cpp \
  src/music.cpp \
  src/state/State.cpp \
  src/view/View.cpp \
  -o "${out_dir}/${project}.wasm"

# generate webpage (HTML only when it does not exist)
generate_html=""
if [ ! -f "${out_dir}/${project}.html" ]; then generate_html="${out_dir}/${project}.html"; fi

flags="-no_minify"
flags="-rle"
node ${wajic_dir}/wajicup.js ${flags} "${out_dir}/${project}.wasm" "${out_dir}/${project}.js" ${generate_html}

rm "${out_dir}/${project}.wasm"

