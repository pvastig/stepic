cmake_minimum_required (VERSION 2.6)

set (PROJECT meta)

project (${PROJECT})

set (HEADERS ../sources/meta.h)
set (SOURCES ../sources/meta.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

#if(NOT CMAKE_MAXIMUM_RECURSION_DEPTH)
#  #set(CMAKE_MAXIMUM_RECURSION_DEPTH 2000)
#  message(${CMAKE_MAXIMUM_RECURSION_DEPTH})
#endif()

add_library (${PROJECT} ${HEADERS} ${SOURCES})
