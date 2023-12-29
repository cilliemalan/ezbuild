cmake_minimum_required(VERSION 3.20)

project(ezbuild)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
add_compile_options(-Wno-microsoft-enum-forward-reference)
endif()

add_library(quickjs
    src/quickjs/quickjs.c
    src/quickjs/libregexp.c
    src/quickjs/libunicode.c
    src/quickjs/cutils.c
    src/quickjs/libbf.c)
target_compile_definitions(quickjs PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(quickjs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/quickjs)

add_executable(ezbuild
    src/ezbuild.c)
target_link_libraries(ezbuild PRIVATE quickjs)
