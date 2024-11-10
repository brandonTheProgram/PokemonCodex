cmake_minimum_required(VERSION 3.14)
project(PokemonCodex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SQLiteCpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/cpp-httplib)

include_directories(
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/cpp-httplib
)

set(SOURCES
    main.cpp
    source/SQLManager.cpp
    source/Pokedex.cpp
    source/HttpRouter.cpp
)

add_executable(PokemonCodex ${SOURCES})

target_compile_definitions(PokemonCodex PRIVATE DATABASE_PATH="${CMAKE_SOURCE_DIR}/db/Data/Pokedex.db")

target_link_libraries(PokemonCodex PRIVATE SQLiteCpp jsoncpp_static)