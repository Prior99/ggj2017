cmake_minimum_required(VERSION 3.0)
project(ggj2017)

add_definitions("-g -Wall -Wextra -pedantic -Werror -std=c++14 -Wno-unused-parameter")

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)

add_executable(ggj2017 ${sources})

if(${EMSCRIPTEN})
    set(CROSS_COMPILING ON)
    add_definitions("-Oz -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'")
    set(CMAKE_CXX_LINK_FLAGS "-pg -Oz -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s TOTAL_MEMORY=128000000 --emrun --preload-file ../res -o index.html")
else()
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image SDL2_mixer SDL2_ttf)
endif()

set(ENTITYX_BUILD_TESTING false CACHE BOOL "")
add_subdirectory(entityx)

include_directories(SYSTEM
    ${SDL2_INCLUDE_DIRS}
    glm/
    entityx/
    strapon/
    src/
)

target_link_libraries(ggj2017
    ${SDL2_LIBRARIES}
    entityx
    pulse-simple
    pulse
)
