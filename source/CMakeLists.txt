cmake_minimum_required(VERSION 3.14)

set(SOURCES
    main.cpp
    HttpRouter.cpp
    Logger.cpp
    Pokedex.cpp
    SQLManager.cpp
)

add_library(PokemonCodexLib ${SOURCES})

target_include_directories(PokemonCodexLib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/external/cpp-httplib
    ${CMAKE_SOURCE_DIR}/external/SQLiteCpp/include
    ${CMAKE_SOURCE_DIR}/external/jsoncpp/include
)

target_link_libraries(PokemonCodexLib PRIVATE SQLiteCpp jsoncpp_static)

add_executable(PokemonCodex ${SOURCES})

target_compile_definitions(PokemonCodexLib PUBLIC DATABASE_PATH="${CMAKE_SOURCE_DIR}/db/Data/Pokedex.db")

target_link_libraries(PokemonCodex PokemonCodexLib)