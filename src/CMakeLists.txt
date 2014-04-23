cmake_minimum_required (VERSION 2.8)
project (TrippingWookie)
find_package (OpenCV REQUIRED)

if (UNIX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
endif(UNIX)

include_directories ("${PROJECT_SOURCE_DIR}/Gesture")
add_subdirectory (Gesture)

add_executable (TrippingWookie TrippingWookie.cpp)
target_link_libraries (TrippingWookie ${OpenCV_LIBS} Gesture)
