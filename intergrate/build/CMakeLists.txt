cmake_minimum_required (VERSION 2.6)

set (PROJECT intergrate)

project (${PROJECT})

set (HEADERS ../sources/intergrate.h)
set (SOURCES ../sources/intergrate.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
