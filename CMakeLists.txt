cmake_minimum_required(VERSION 3.28)

project("libBioSignalML")
set(BioSignalML_VERSION_MAJOR 0)
set(BioSignalML_VERSION_MINOR 8)

set(BioSignalML_VERSION ${BioSignalML_VERSION_MAJOR}.${BioSignalML_VERSION_MINOR})
set(BioSignalML_VERSION_SO ${BioSignalML_VERSION_MAJOR})

### CMake helpers...
include(CMakePackageConfigHelpers)

if (UNIX)
  add_definitions(-std=c++11)  # Use C++11
  add_definitions(-Wall -Wextra -Wstrict-aliasing -Wstrict-overflow)
  add_definitions(-Wno-unused-parameter)
elseif(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -wd4251)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Building DEBUG version")
else()
  message(STATUS "Building RELEASE version")
endif()

set(INCLUDES $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

if("${typedobject_LIBRARY}" STREQUAL "")
  find_package(typedobject REQUIRED)
endif()
set(INCLUDES ${INCLUDES} ${typedobject_INCLUDE_DIR})
set(LIBRARIES ${typedobject_LIBRARY})

include(${typedobject_DIR}/typedobject_generate.cmake)

set(HEADERS ${CMAKE_SOURCE_DIR}/include/biosignalml/resource.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/recording.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/signal.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/segment.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/timing.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/event.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/annotation.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/data/data.h
            ${CMAKE_SOURCE_DIR}/include/biosignalml/data/hdf5.h
            )
add_subdirectory(src)

foreach(file ${HEADERS})
  typedobject_generate(GENERATED_FILES ${file} "${INCLUDES}")
endforeach()
set(SOURCES ${SOURCES} ${GENERATED_FILES})

add_library(biosignalml SHARED ${SOURCES})

target_link_libraries(biosignalml PUBLIC ${LIBRARIES})
target_include_directories(biosignalml PUBLIC ${INCLUDES})

enable_testing()
add_subdirectory(tests)


set_property(TARGET biosignalml PROPERTY VERSION ${BioSignalML_VERSION})
set_property(TARGET biosignalml PROPERTY SOVERSION ${BioSignalML_VERSION_SO})
set_property(TARGET biosignalml PROPERTY INTERFACE_biosignalml_VERSION_MAJOR ${BioSignalML_VERSION_MAJOR})
set_property(TARGET biosignalml APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING biosignalml_VERSION_MAJOR
  )

install(TARGETS biosignalml EXPORT biosignalmlTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
  )

install(
  DIRECTORY
    ${CMAKE_SOURCE_DIR}/include/biosignalml
  DESTINATION
    include
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
  @ONLY
  )

# Where package configuration files are put
set(ConfigPackageLocation lib/cmake/biosignalml)

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
    "${CMAKE_CURRENT_BINARY_DIR}/biosignalml/biosignalmlConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/biosignalml/biosignalmlConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
  )

# Ensure symbols are stripped from packaged libraries
install(SCRIPT cmake/strip_libraries.cmake)

### Build a package...
if(WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_PACKAGE_VERSION_MAJOR ${BioSignalML_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${BioSignalML_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION ${BioSignalML_VERSION})
include(CPack)
