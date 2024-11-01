cmake_minimum_required(VERSION 3.14)
project(PokemonCodex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SQLiteCpp)

include_directories(include)

set(SOURCES
    main.cpp
    source/SQLManager.cpp
)

add_executable(PokemonCodex ${SOURCES})

target_compile_definitions(PokemonCodex PRIVATE DATABASE_PATH="${CMAKE_SOURCE_DIR}/db/Data/Pokedex.db")

target_link_libraries(PokemonCodex PRIVATE SQLiteCpp)