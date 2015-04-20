cmake_minimum_required(VERSION 3.0)
project("BioSignalML Library in C++")

# We use C++11
if (UNIX)
  add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(typedobject_DIR ${CMAKE_SOURCE_DIR}/external/typedobject)
add_subdirectory(${typedobject_DIR})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

