
#CMake 最低版本号要求
cmake_minimum_required(VERSION 3.0.0)
#指定工程名
project(MiniOglEngine VERSION 0.1.0)
#指定C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#指定C/C++编译器

set(CMAKE_C_COMPILER "D:/software/00-SmallTools/mingw64/bin/gcc")
set(CMAKE_CXX_COMPILER "D:/software/00-SmallTools/mingw64/bin/g++")
add_definitions("-Wall -g")

#
find_package(jsoncpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${JSONCPP_INCLUDE_DIR}\"")
#
# configure_file(MiniOglEngineConfig.h.in MiniOglEngineConfig.h)

set(THIRD_PARTY_ROOT ${PROJECT_SOURCE_DIR}/engine/third-party)
set(OPENGL_ROOT ${THIRD_PARTY_ROOT}/opengl)
set(OPENCV_ROOT ${THIRD_PARTY_ROOT}/opencv)
#指定头文件所在的路径
include_directories(${PROJECT_SOURCE_DIR}/engine/source/runtime/include ${OPENGL_ROOT}/includes ${OPENCV_ROOT}/includes)




#指定生成的可执行文件夹的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)



#在 ${PROJECT_SOURCE_DIR}/src 路径下搜索.cpp源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/engine/source/runtime ALL_SRCS)
#生成可执行文件 main.exe
LINK_DIRECTORIES(${OPENGL_ROOT}/lib ${OPENCV_ROOT}/lib D:/resources/mylibrary/dll)
add_executable(${PROJECT_NAME} ${ALL_SRCS})
target_link_libraries(${PROJECT_NAME} glfw3 jsoncpp opengl32 opencv_core452 opencv_imgcodecs452)

# target_include_directories(${PROJECT_NAME} PUBLIC
#                            ${PROJECT_BINARY_DIR}
#                            )


