cmake_minimum_required(VERSION 3.6)

project(MY_APP)

find_library(SDL2 SDL2)

add_library(main SHARED)

target_sources(main PRIVATE src/main.c src/game.c src/misc.c src/sprite.c)

target_link_libraries(main SDL2)


