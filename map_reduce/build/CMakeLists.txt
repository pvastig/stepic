cmake_minimum_required (VERSION 2.6)

set (PROJECT map_reduce)

project (${PROJECT})

set (HEADERS ../sources/map_reduce.h)
set (SOURCES ../sources/map_reduce.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
