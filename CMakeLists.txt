cmake_minimum_required(VERSION 3.14)
project(PokemonCodex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sqlite3 STATIC
    external/sqlite3/sqlite3.c
)

target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite3)

set(SOURCES
    main.cpp
)

add_executable(PokemonCodex ${SOURCES})

target_link_libraries(PokemonCodex PRIVATE sqlite3)