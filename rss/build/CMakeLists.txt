cmake_minimum_required (VERSION 2.6)

set (PROJECT rss)

project (${PROJECT})

set (HEADERS ../sources/rss.h)
set (SOURCES ../sources/rss.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
