cmake_minimum_required(VERSION 3.10)
project(HDiffLib)

file(GLOB_RECURSE HEADER_FILES "src/*.h")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")

add_library(HdiffLib STATIC ${HEADER_FILES} ${SOURCE_FILES})

set(OUT_DIR "${CMAKE_SOURCE_DIR}/HDiffPatch")

add_custom_command(TARGET HdiffLib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/include/hdiff/HDiffPatch.h ${OUT_DIR}/include/hdiff/HDiffPatch.h
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:HdiffLib> ${OUT_DIR}/bin/HDiffPatch.lib
    COMMENT "Copying library and header file to output directory"
)