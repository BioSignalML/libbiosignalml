cmake_minimum_required(VERSION 3.0)

project("libBioSignalML")
set(BioSignalML_VERSION 0.0.5)

### CMake helpers...
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)


if (UNIX)
  add_definitions(-std=c++11)  # Use C++11
elseif(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(INCLUDES $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
             $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
             )

FIND_PACKAGE(typedobject REQUIRED)
set(INCLUDES ${INCLUDES} ${typedobject_INCLUDE_DIR})
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

generate_export_header(biosignalml
                       EXPORT_FILE_NAME
                       ${CMAKE_BINARY_DIR}/include/biosignalml_export.h)

enable_testing()
add_subdirectory(tests)


set_property(TARGET biosignalml PROPERTY VERSION ${BioSignalML_VERSION})
set_property(TARGET biosignalml PROPERTY SOVERSION 3)
set_property(TARGET biosignalml PROPERTY INTERFACE_biosignalml_MAJOR_VERSION 0)
set_property(TARGET biosignalml APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING biosignalml_MAJOR_VERSION
  )

install(TARGETS biosignalml EXPORT biosignalmlTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
  )

install(
  DIRECTORY src/
  DESTINATION include/biosignalml
  COMPONENT Devel
  FILES_MATCHING PATTERN "*.h"
  )

install(
  FILES
    src/data/data.h
    src/data/hdf5.h
  DESTINATION include/biosignalml/data
  COMPONENT Devel
  )

install(
  FILES
    ${CMAKE_BINARY_DIR}/include/biosignalml_export.h
  DESTINATION
    include/biosignalml
  COMPONENT
    Devel
  )

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/biosignalml/biosignalmlConfigVersion.cmake"
  VERSION ${BioSignalML_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

export(EXPORT biosignalmlTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/biosignalml/biosignalmlTargets.cmake"
  NAMESPACE BioSignalML::
  )
configure_file(cmake/biosignalmlConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/biosignalml/biosignalmlConfig.cmake"
  COPYONLY
  )

if(WIN32)
  set(ConfigPackageLocation cmake)
else()
  set(ConfigPackageLocation lib/cmake/biosignalml)
endif()

install(EXPORT biosignalmlTargets
  FILE
    biosignalmlTargets.cmake
  NAMESPACE
    BioSignalML::
  DESTINATION
    ${ConfigPackageLocation}
  )

install(
  FILES
    cmake/biosignalmlConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/biosignalml/biosignalmlConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
  )
