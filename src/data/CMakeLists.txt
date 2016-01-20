# Specify HDF5 library/includes...
if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-DH5_DEBUG=1)
endif()

## There are conflicts between the version of `FindHDF5.cmake` released
## with CMake 3.4 and that released with HDF5 1.8.16, so we use `hdf5-config.cmake`
## from the HDF5 release and augment its results. This requires that `HDF5_DIR` is
## set to the location of `hdf5-config.cmake`.
FIND_PACKAGE(HDF5 "1.8" REQUIRED COMPONENTS static C CXX HL CXX_HL NO_MODULE)
set(HDF5_LIBRARY_DIRS ${HDF5_BUILD_DIR}/lib)
if(WIN32)
  set(HDF5_LIBRARIES hdf5 hdf5_cpp hdf5_hl hdf5_hl_cpp szip zlib)
  set(LIBRARY_PREFIX "lib")
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(LIBRARY_POSTFIX "_D")
  endif()
else()
  set(HDF5_LIBRARIES hdf5 hdf5_cpp hdf5_hl hdf5_hl_cpp szip z)
endif()
foreach(library ${HDF5_LIBRARIES})
  unset(LIBNAME CACHE)
  find_library(LIBNAME ${LIBRARY_PREFIX}${library}${LIBRARY_POSTFIX} PATHS ${HDF5_LIBRARY_DIRS})
  set(HDF5_LIBS ${HDF5_LIBS} ${LIBNAME})
endforeach()

set(INCLUDES ${INCLUDES} ${HDF5_INCLUDE_DIR} PARENT_SCOPE)

set(LIBRARIES ${LIBRARIES}
              ${HDF5_LIBS}
              PARENT_SCOPE)

set(SOURCES ${SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/timeseries.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5impl.cpp
            PARENT_SCOPE)
