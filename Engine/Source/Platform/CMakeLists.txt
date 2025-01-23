project(Platform)
file(GLOB_RECURSE PLATFORM_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(Platform STATIC ${PLATFORM_SRC_FILES})

target_include_directories(Platform
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public/
)
