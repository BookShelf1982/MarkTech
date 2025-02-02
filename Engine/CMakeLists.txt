cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STANDARD_REQUIRED True)

set(VULKAN_INCLUDE_DIR C:/VulkanSDK/1.3.275.0/Include)
set(CORE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Public/)
set(ASSETDB_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Source/AssetDB/Public/)
set(PLATFORM_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Public/)
set(INIH_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/inih/)
set(XML_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/xml/)

project(MarkTech)

if(WIN32)
add_compile_definitions(MT_PLATFORM_WINDOWS)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/AssetDB/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/inih/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/xml/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Volk/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Sandbox/)