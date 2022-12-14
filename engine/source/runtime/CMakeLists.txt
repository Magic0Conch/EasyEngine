project(Rasterizaion)

find_package(OpenGL REQUIRED)
set(SOURCE_ROOT_DIR "${ENGINE_ROOT_DIR}/source")
set(RUNTIME_ROOT_DIR "${SOURCE_ROOT_DIR}/runtime")
find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs)
if(OpenCV_FOUND)
message(STATUS "The Opencv lib is found!") 
message(STATUS "    opencv dir: ${OpenCV_INCLUDE_DIRS}")
else()
message(STATUS "The Opecv lib is not found!Set OpencvDir please!") 

endif()


# Create a sources variable with a link to all cpp files to compile
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})
# message(STATUS "source: ${SOURCE_FILES}") 
# message(STATUS "header: ${HEADER_FILES}") 
# Add an executable with the above sources
add_executable(${PROJECT_NAME}  ${HEADER_FILES} ${SOURCE_FILES})


target_include_directories(${PROJECT_NAME}
    PUBLIC
        external
        ${OPENGL_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}        
)


target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${OPENGL_gl_LIBRARY}
        # ${OpenCV_LIBRARIES}
        glfw
        glad
        glm
        jsoncpp_static    
        opencv_core
        opencv_highgui
        opencv_imgcodecs
        #glm_static    # if build with add_subdirectory
)

# set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
# set(EASYENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# set(CMAKE_INSTALL_PREFIX "${EASYENGINE_ROOT_DIR}/bin")
# set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/")

set(ENGINE_ASSET_DIR "asset")
set(ENGINE_CONFIG_DIR "configs")
set(ENGINE_SHADER_DIR "shader")
set(GLOBAL_CONFIG "global_config")
message(STATUS "${ENGINE_ROOT_DIR}/${ENGINE_ASSET_DIR}")
set(POST_BUILD_COMMANDS
  COMMAND ${CMAKE_COMMAND} -E make_directory "${BINARY_ROOT_DIR}"
  # COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/${ENGINE_ASSET_DIR}" "${ENGINE_ROOT_DIR}/${ENGINE_ASSET_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/" "${BINARY_ROOT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "${ENGINE_ROOT_DIR}/${ENGINE_CONFIG_DIR}/${GLOBAL_CONFIG}.ini" "${BINARY_ROOT_DIR}"
#   COMMAND ${CMAKE_COMMAND} -E copy "${ENGINE_ROOT_DIR}/${DEVELOP_CONFIG_DIR}/${PROJECT_NAME}.ini" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${BINARY_ROOT_DIR}/${ENGINE_ASSET_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/${ENGINE_ASSET_DIR}" "${BINARY_ROOT_DIR}/${ENGINE_ASSET_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/${ENGINE_SHADER_DIR}" "${BINARY_ROOT_DIR}/${ENGINE_SHADER_DIR}"
)
# set(TARGET_NAME STATIC_RESOURCES)


add_custom_command(TARGET ${PROJECT_NAME} ${POST_BUILD_COMMANDS})