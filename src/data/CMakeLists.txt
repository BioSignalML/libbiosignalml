# Specify HDF5 library/includes...


#set(HDF5_USE_STATIC_LIBRARIES 1)


add_definitions(-D_HDF5USEDLL_)

if(WIN32)
  FIND_PACKAGE(HDF5 "1.8" REQUIRED NO_MODULE COMPONENTS C CXX HL)
else()
  FIND_PACKAGE(HDF5 "1.8" REQUIRED           COMPONENTS C CXX HL)
endif()

set(INCLUDES ${INCLUDES} ${HDF5_INCLUDE_DIR} PARENT_SCOPE)


set(LIBRARIES ${LIBRARIES}
              ${HDF5_LIBRARIES}
              PARENT_SCOPE)


set(SOURCES ${SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5impl.cpp
            PARENT_SCOPE)

set(HEADERS ${HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/model.h
            ${CMAKE_CURRENT_SOURCE_DIR}/hdf5.h
            PARENT_SCOPE)

