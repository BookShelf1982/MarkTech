project(AssetDB)
file(GLOB_RECURSE ASSETDB_SRC_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_executable(AssetDB ${ASSETDB_SRC_FILES})

target_include_directories(AssetDB
	PUBLIC ${XML_INCLUDE_PATH}
	PUBLIC ${PLATFORM_INCLUDE_PATH}
	PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/stb/
)

target_link_libraries(AssetDB xml)