cmake_minimum_required(VERSION 3.10)
project(HDiffLib)

file(GLOB_RECURSE HEADER_FILES "src/*.h")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")

add_executable(HdiffLib ${HEADER_FILES} ${SOURCE_FILES})