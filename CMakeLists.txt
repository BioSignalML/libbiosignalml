cmake_minimum_required(VERSION 3.0)
project("BioSignalML Library in C++")

# We use C++11
if (UNIX)
  add_definitions(-std=c++11)
endif()

set(TYPEDOBJECT ${CMAKE_SOURCE_DIR}/external/typedobject)
add_subdirectory(${TYPEDOBJECT})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

