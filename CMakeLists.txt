cmake_minimum_required(VERSION 3.5)
project(Game)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    libs/nanovg/nanovg.c
    libs/choreograph/Cue.cpp
    libs/choreograph/Timeline.cpp
    libs/choreograph/TimelineItem.cpp
    libs/libxm/src/context.c
    libs/libxm/src/load.c
    libs/libxm/src/play.c
    libs/libxm/src/xm.c
    resources/resources.inc
    src/platform/platform_glfw.cpp
    src/framework/ImageFile.cpp
    src/framework/Target.cpp
    src/framework/ShaderProgram.cpp
    src/framework/SpriteAtlas.cpp
    src/framework/SpriteBatch.cpp
    src/framework/Texture.cpp
    src/framework/get_resource.cpp
    src/Entity.cpp
    src/Actor.cpp
    src/Player.cpp
    src/Pig.cpp
    src/PigKing.cpp
    src/Game.cpp
    src/Graphics.cpp
    src/main.cpp
    src/Object.cpp
    src/World.cpp
)

if(WIN32)
    set(SOURCES ${SOURCES}
        libs/glad/glad_gl.c
    )
endif()

file(GLOB_RECURSE HEADERS include *.h)

add_compile_definitions(XM_DEFENSIVE=0 XM_LIBXMIZE_DELTA_SAMPLES=0 XM_DEBUG=0 XM_LINEAR_INTERPOLATION=0)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
  . src libs
  libs/libxm
)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -fno-rtti -fno-exceptions")
  target_link_libraries(${PROJECT_NAME} GL glfw dl pthread)
else()
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)

  include (ExternalProject)
  ExternalProject_Add(glfw
          GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
          CMAKE_ARGS ${CMAKE_ARGS}
                  -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
                  -DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
                  -DGLFW_BUILD_DOCS=Off
                  -DGLFW_BUILD_TESTS=Off
                  -DGLFW_BUILD_EXAMPLES=Off
          UPDATE_COMMAND ""
  )
  add_dependencies(${PROJECT_NAME} glfw)
  target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
  target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/lib)
  target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)
endif()

if(NOT WIN32)
  install(TARGETS ${PROJECT_NAME} DESTINATION "bin")
else(NOT WIN32)
  install(TARGETS ${PROJECT_NAME} DESTINATION .)
endif(NOT WIN32)
