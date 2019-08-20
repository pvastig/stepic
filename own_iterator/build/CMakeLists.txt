cmake_minimum_required (VERSION 2.6)

set (PROJECT own_iterator)

project (${PROJECT})

set (HEADERS ../sources/own_iterator.h)
set (SOURCES ../sources/own_iterator.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
