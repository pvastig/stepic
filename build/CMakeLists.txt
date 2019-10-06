cmake_minimum_required (VERSION 2.6)

set (PROJECT stepic)

project (${PROJECT})
set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

include_directories (../)

set (LIBRARIES
		intergrate
                map_reduce
		meta
		own_iterator
		rss)

foreach (LIBRARY ${LIBRARIES})
	add_subdirectory (../${LIBRARY}/build bin/${LIBRARY})
endforeach ()

if (MSVC)
	add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions (-Wall -pedantic)
else ()
	message ("Unknown compiler")
endif ()

set (HEADERS)
set (SOURCES ../main.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_executable (${PROJECT} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT} Threads::Threads)

target_link_libraries (${PROJECT} ${LIBRARIES})
