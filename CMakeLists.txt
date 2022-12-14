cmake_minimum_required(VERSION 3.0.0)

project(EasyEngine VERSION 0.1.0 LANGUAGES CXX C)
# set(CMAKE_CXX_FLAGS "-lstdc++fs")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#export config to clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# include(CMakeDependentOption)

# if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
#   message(
#     FATAL_ERROR
#       "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
#   )
# endif()

set(EASYENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(EASYENGINE_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_INSTALL_PREFIX "${EASYENGINE_BUILD_DIR}/bin")
set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/")

add_subdirectory(engine)