project(Sandbox)
add_executable(Sandbox 
	${CMAKE_CURRENT_SOURCE_DIR}/Private/Main.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/Private/imgui_impl_vulkan.h
	#${CMAKE_CURRENT_SOURCE_DIR}/Private/imgui_impl_win32.h
	#${CMAKE_CURRENT_SOURCE_DIR}/Private/imgui_impl_vulkan.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/Private/imgui_impl_win32.cpp
)

target_include_directories(Sandbox
    PUBLIC ${PLATFORM_INCLUDE_PATH}
    PUBLIC ${CORE_INCLUDE_PATH}
    PUBLIC ${ASSETDB_INCLUDE_PATH}
)

target_link_libraries(Sandbox
    Platform
    Core
	AssetDB
)