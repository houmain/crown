cmake_minimum_required(VERSION 3.5)
project(Game)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/platform/platform_glfw.cpp
    src/main.cpp
    src/Game.cpp
    src/music.cpp
    src/state/State.cpp
    src/view/View.cpp
    src/framework/ImageFile.cpp
    src/framework/Target.cpp
    src/framework/ShaderProgram.cpp
    src/framework/SpriteAtlas.cpp
    src/framework/SpriteBatch.cpp
    src/framework/Texture.cpp
    src/framework/get_resource.cpp
    libs/nanovg/nanovg.c
    libs/choreograph/Cue.cpp
    libs/choreograph/Timeline.cpp
    libs/choreograph/TimelineItem.cpp
    libs/libxm/src/context.c
    libs/libxm/src/load.c
    libs/libxm/src/play.c
    libs/libxm/src/xm.c
    resources/resources.inc
)

file(GLOB_RECURSE HEADERS include *.h)

add_compile_definitions(XM_DEFENSIVE=0 XM_LIBXMIZE_DELTA_SAMPLES=0 XM_DEBUG=0 XM_LINEAR_INTERPOLATION=0)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
  . src libs
  libs/libxm
)

target_link_libraries(${PROJECT_NAME} GL glfw dl pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")
