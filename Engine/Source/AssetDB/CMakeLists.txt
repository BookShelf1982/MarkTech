project(AssetDB)
file(GLOB_RECURSE ASSETDB_SRC_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(AssetDB STATIC ${ASSETDB_SRC_FILES})

target_include_directories(AssetDB
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public/
	PUBLIC ${INIH_INCLUDE_PATH}
)

target_link_libraries(AssetDB inih)