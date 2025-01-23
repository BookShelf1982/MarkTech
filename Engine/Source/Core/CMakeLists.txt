project(Core)
file(GLOB_RECURSE CORE_SRC_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(Core STATIC ${CORE_SRC_FILES})

target_include_directories(Core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public/
)