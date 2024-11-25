cmake_minimum_required(VERSION 3.14)
project(PokemonCodexTests)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE # Disable preserving timestamps
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
    LoggerTests.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(UnitTests ${TEST_SOURCES})

target_link_libraries(UnitTests PRIVATE 
    gtest 
    gtest_main
    PokemonCodexLib
)

enable_testing()
add_test(NAME UnitTests COMMAND UnitTests)