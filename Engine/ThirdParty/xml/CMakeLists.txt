project(xml)
add_library(xml STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/xml.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xml.h
)
target_include_directories(xml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/xml.h)