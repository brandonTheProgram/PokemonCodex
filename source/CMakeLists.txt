cmake_minimum_required(VERSION 3.14)

set(SOURCES
    main.cpp
    HttpRouter.cpp
    Logger.cpp
    Pokedex.cpp
    SQLManager.cpp
    Config.cpp
    repositories/AttackRepository.cpp
    repositories/GameRepository.cpp
    repositories/PokemonRepository.cpp
    repositories/RegionRepository.cpp
    repositories/TypeRepository.cpp
)

add_library(PokemonCodexLib ${SOURCES})


target_include_directories(PokemonCodexLib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/external/cpp-httplib
    ${CMAKE_SOURCE_DIR}/external/SQLiteCpp/include
    ${CMAKE_SOURCE_DIR}/external/jsoncpp/include
)

# Link libraries to PokemonCodexLib
target_link_libraries(PokemonCodexLib PRIVATE SQLiteCpp jsoncpp_static)

add_executable(PokemonCodex ${SOURCES})

# Link libraries to the executable
target_link_libraries(PokemonCodex PokemonCodexLib)

if(CLANG_FORMAT_EXECUTABLE)
    message(STATUS "clang-format found! Enabling code formatting.")
    
    add_custom_target(
        format
        COMMAND ${CMAKE_COMMAND} -E echo "Running clang-format"
        COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/source/*.cpp ${CMAKE_SOURCE_DIR}/include/*.h
        COMMENT "Running clang-format to fix code style"
    )

    # Ensure format runs before building the executable
    add_dependencies(PokemonCodex format)

    # Run format as a PRE_BUILD command to ensure it's done before building
    add_custom_command(
        TARGET PokemonCodex PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Running clang-format before building executable"
        COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/source/*.cpp ${CMAKE_SOURCE_DIR}/include/*.h
        COMMENT "Running clang-format before building executable"
    )
endif()