cmake_minimum_required(VERSION 3.0)
project("BioSignalML Library in C++")

if (UNIX)
  add_definitions(-std=c++11)  # Use C++11
elseif(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-DBOOST_DATE_TIME_NO_LIB)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(typedobject_DIR ${CMAKE_SOURCE_DIR}/external/typedobject)
add_subdirectory(${typedobject_DIR})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

