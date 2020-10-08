cmake_minimum_required(VERSION 3.5.1)
project(libbmp VERSION 1.0.0 LANGUAGES C)

# Set output to appropriate directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Find source files
file(GLOB SOURCES src/*.c)

# Include header files
include_directories(include)
include(GNUInstallDirs)

# Create shared library
# add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
# set(DEFAULT_BUILD_TYPE "Release")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:RELEASE>)
