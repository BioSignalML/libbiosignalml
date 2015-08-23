### Find Boost unit testing package:
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
if(WIN32)
  add_definitions(-DBOOST_ALL_NO_LIB)
endif()
find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_definitions (-DBOOST_TEST_DYN_LINK)
include_directories(${Boost_INCLUDE_DIRS})
###

set(LIBRARIES biosignalml
              ${Boost_FILESYSTEM_LIBRARY}
              ${Boost_SYSTEM_LIBRARY}
              ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
              )

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


add_executable(unittest unittest.cpp)
target_link_libraries(unittest ${LIBRARIES})
add_test(UNITTEST, unittest)


add_executable(test_bsml bsml.cpp)
target_link_libraries(test_bsml biosignalml)
add_test(BSML, test_bsml)

add_executable(test_hdf5 hdf5.cpp)
target_link_libraries(test_hdf5 biosignalml)
add_test(HDF5, test_hdf5)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(test_hdf5impl hdf5impl.cpp)
target_link_libraries(test_hdf5impl biosignalml)
add_test(HDF5, test_hdf5impl)

