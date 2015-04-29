# Specify HDF5 library/includes...

FIND_PACKAGE(HDF5 "1.8" REQUIRED)
set(INCLUDES ${INCLUDES} ${HDF5_INCLUDE_DIR} PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} hdf5 hdf5_cpp PARENT_SCOPE)

set(SOURCES ${SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5impl.cpp
            PARENT_SCOPE)

set(HEADERS ${HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/model.h
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5.h
            PARENT_SCOPE)

