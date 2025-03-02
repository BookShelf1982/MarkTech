cmake_minimum_required(VERSION 3.25)
set(CMAKE_C_STANDARD 11)
set(CMAKE_STANDARD_REQUIRED True)

project(MazeWalker)

add_subdirectory(C:/MazeWalker/ThirdParty/SPIRV-Cross)

set(VULKAN_SDK_DIR C:/VulkanSDK/1.3.275.0 CACHE PATH "The root directory of the Vulkan SDK")

add_library(imgui STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imconfig.h
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imgui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imgui.h
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imgui_demo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imgui_internal.h
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imgui_widgets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imstb_rectpack.h
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imstb_textedit.h
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/imstb_truetype.h
)

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

add_library(math3D
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Math3D/math3D.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Math3D/math3D.cpp
)

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
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/window.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/renderer.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/resource_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/resource_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/alloc_uitls.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/djb2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/djb2.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/gameplay.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/gameplay.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/imgui_impl_win32.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/imgui_impl_win32.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/imgui_impl_opengl3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/imgui_impl_opengl3.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/imgui_impl_opengl3_loader.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/data_structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/dev_console.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/dev_console.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/dev_gui.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/dev_gui.cpp
)

target_compile_definitions(mzwk PRIVATE
	# If the debug configuration pass the DEBUG define to the compiler 
	$<$<CONFIG:Debug>:DEBUG>
)

target_link_directories(mzwk PUBLIC ${VULKAN_SDK_DIR}/Lib)

target_include_directories(mzwk 
	PUBLIC ${VULKAN_SDK_DIR}/Include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/FileSystem
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/Math3D
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui
)

target_link_libraries(mzwk 
	glad
	opengl32.lib
	spirv-cross-glsl
	filesystem
	math3D
	imgui
)