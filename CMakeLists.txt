cmake_minimum_required(VERSION 3.14)

project(Tracer
    VERSION 1.0
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# link options
add_link_options(-rdynamic)

# Create a library from the source files
add_library(tracer ${SOURCES})

# Link tracer with dl
target_link_libraries(tracer dl)

# Example application
add_executable(main example/main.cpp)

# Link the example app with the library
target_link_libraries(main PRIVATE tracer)
