cmake_minimum_required(VERSION 3.10)
include(FindPackageHandleStandardArgs)


project(Himpapawid)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")

find_package(SDL2 2.0 REQUIRED)

file(GLOB SOURCES "src/*.c")

include_directories(
    ${SDL2_INCLUDE_DIR}
)

link_libraries(
    ${SDL2_LIBRARY}
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(himpapawid ${SOURCES})