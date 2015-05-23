cmake_minimum_required(VERSION 3.0)
project("BioSignalML Library in C++")

if (UNIX)
  add_definitions(-std=c++11)  # Use C++11
elseif(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
FIND_PACKAGE(typedobject REQUIRED)
set(INCLUDES ${typedobject_INCLUDE_DIR})
find_library(typedobject_LIB typedobject PATHS ${typedobject_LIBRARY_DIR})
set(LIBRARIES ${typedobject_LIB})

include(${typedobject_DIR}/typedobject_generate.cmake)

add_subdirectory(src)

foreach(file ${HEADERS})
  typedobject_generate(GENERATED_FILES ${file} "${INCLUDES}")
endforeach()
set(SOURCES ${SOURCES} ${GENERATED_FILES})

add_library(biosignalml SHARED ${SOURCES})

target_link_libraries(biosignalml ${LIBRARIES})
target_include_directories(biosignalml PUBLIC ${INCLUDES})
enable_testing()
add_subdirectory(tests)

