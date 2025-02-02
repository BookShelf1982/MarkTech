project(Platform)

add_library(FileSystem SHARED 
    ${CMAKE_CURRENT_SOURCE_DIR}/Private/FileSystem/FileSystem.cpp
)

target_compile_definitions(FileSystem PRIVATE FS_DLL)

target_include_directories(FileSystem
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public/FileSystem/
)

if(WIN32)
target_link_libraries(FileSystem shlwapi.lib)
endif()