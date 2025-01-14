cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STANDARD_REQUIRED True)

set(VULKAN_INCLUDE_DIR C:/VulkanSDK/1.3.275.0/Include)

# Volk Project
project(Volk)
add_library(Volk STATIC 
	${CMAKE_SOURCE_DIR}/Engine/ThirdParty/Volk/volk.c
	${CMAKE_SOURCE_DIR}/Engine/ThirdParty/Volk/volk.h
)
target_include_directories(Volk
	PUBLIC ${VULKAN_INCLUDE_DIR}
)

if(WIN32)
target_compile_definitions(Volk
	PUBLIC VK_USE_PLATFORM_WIN32_KHR
)
endif()

# inih Project
project(inih)
add_library(inih STATIC 
	${CMAKE_SOURCE_DIR}/Engine/ThirdParty/inih/ini.c
	${CMAKE_SOURCE_DIR}/Engine/ThirdParty/inih/ini.h
)

# Sandbox Project
project(Sandbox)
file(GLOB_RECURSE SANDBOX_SRC_FILES 
	${CMAKE_SOURCE_DIR}/Engine/Source/Runtime/Sandbox/Private/*.cpp
	${CMAKE_SOURCE_DIR}/Engine/Source/Runtime/Sandbox/Private/*.h
)

add_executable(Sandbox ${SANDBOX_SRC_FILES})
target_link_libraries(Sandbox 
	PUBLIC Volk
	PUBLIC inih
)

target_include_directories(Sandbox
	PUBLIC ${VULKAN_INCLUDE_DIR}
	PUBLIC ${CMAKE_SOURCE_DIR}/Engine/Source/Runtime/Sandbox/Private/
	PUBLIC ${CMAKE_SOURCE_DIR}/Engine/ThirdParty/Volk/
	PUBLIC ${CMAKE_SOURCE_DIR}/Engine/ThirdParty/inih/
	PUBLIC ${CMAKE_SOURCE_DIR}/Engine/ThirdParty/stb/
)

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dbg_res/)

if(WIN32)
target_compile_definitions(Sandbox
	PUBLIC MT_PLATFORM_WINDOWS
)
set_target_properties(Sandbox PROPERTIES WIN32_EXECUTABLE TRUE)
endif()