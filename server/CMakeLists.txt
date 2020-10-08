cmake_minimum_required(VERSION 3.5.1)
project(server VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Set output to appropriate directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Find source files
file(GLOB SOURCES src/*)

# Include header files
include_directories(include)
include(GNUInstallDirs)

# link_directories("/home/clion_user/casablanca/build.debug/Release/Binaries/")

add_executable(${PROJECT_NAME} ${SOURCES} src/page.cpp include/page.h src/page_manager.cpp include/page_manager.h include/common.h include/user.h include/user_manager.h src/user.cpp)

# target_link_libraries(${PROJECT_NAME} -static -lboost_system libcpprest.a libcrypto.a libz.a -lssl -ldl -lpthread)

target_link_libraries(${PROJECT_NAME} -lpthread stdc++fs)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
# set(DEFAULT_BUILD_TYPE "Release")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:RELEASE>)
