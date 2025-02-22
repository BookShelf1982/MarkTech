cmake_minimum_required(VERSION 3.25)
project(MazeWalker)

add_subdirectory(C:/MazeWalker/ThirdParty/SPIRV-Cross)

set(VULKAN_SDK_DIR C:/VulkanSDK/1.3.275.0 CACHE PATH "The root directory of the Vulkan SDK")

add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glad/src/glad_wgl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glad/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glad/include/glad/glad_wgl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glad/include/KHR/khrplatform.h
)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Glad/include)
 
add_library(filesystem STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/FileSystem/FileSystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/FileSystem/FileSystem.c
)

target_link_libraries(filesystem shlwapi.lib)

add_executable(tool 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Tool/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Tool/djb2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Tool/djb2.c
)

target_include_directories(tool 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb_image
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/FileSystem
)

target_link_libraries(tool filesystem)

add_executable(mzwk 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/window.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/renderer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/hash_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/hash_table.h
)

target_compile_definitions(mzwk PRIVATE
    # If the debug configuration pass the DEBUG define to the compiler 
    $<$<CONFIG:Debug>:DEBUG>
)

target_link_directories(mzwk PUBLIC ${VULKAN_SDK_DIR}/Lib)
target_include_directories(mzwk PUBLIC ${VULKAN_SDK_DIR}/Include)
target_link_libraries(mzwk 
    glad
    opengl32.lib
    spirv-cross-c
)