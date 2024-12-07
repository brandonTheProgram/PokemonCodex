cmake_minimum_required(VERSION 3.14)
project(PokemonCodex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(ENV{LOG_LEVEL} "INFO")
set(ENV{LOG_DIR} "../logs")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SQLiteCpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # Treat warnings as errors for MSVC
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") # Treat warnings as errors for other compilers
endif()

add_subdirectory(source)

add_subdirectory(tests)