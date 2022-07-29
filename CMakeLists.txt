cmake_minimum_required(VERSION 3.10)
include(FindPackageHandleStandardArgs)


project(Himpapawid)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")

find_package(SDL2 2.0 REQUIRED)
find_package(SDL2_image 2.0 REQUIRED)

file(GLOB SOURCES "src/*.c")

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

link_libraries(
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
)

add_executable(himpapawid ${SOURCES})