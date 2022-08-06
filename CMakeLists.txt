cmake_minimum_required(VERSION 3.10)
include(FindPackageHandleStandardArgs)


project(Himpapawid)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(SDL2 2.0 REQUIRED)
find_package(SDL2_mixer 2.0 REQUIRED)
find_package(SDL2_image 2.0 REQUIRED)
find_package(SDL2_ttf 2.0 REQUIRED)

file(GLOB SOURCES "src/*.c")

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

link_libraries(
    ${SDL2_LIBRARY}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

add_executable(himpapawid ${SOURCES})