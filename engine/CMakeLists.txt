
set(ENGINE_ROOT_DIR "${EASYENGINE_ROOT_DIR}/engine")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/third-party")
set(ENGINE_ASSET_DIR "/asset")


# set(CMAKE_CXX_FLAGS "-lstdc++fs")
set(SHADER_COMPILE_TARGET EasyEngineShaderCompile)
add_subdirectory(shader)
add_subdirectory(third-party)
add_subdirectory(source/runtime)


