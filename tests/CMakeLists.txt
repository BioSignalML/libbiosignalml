if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(test_bsml bsml.cpp)
add_test(BSML, test_bsml)
target_link_libraries(test_bsml biosignalml)

add_executable(test_hdf5 hdf5.cpp)
target_link_libraries(test_hdf5 biosignalml)
add_test(HDF5, test_hdf5)

add_executable(test_hdf5impl hdf5impl.cpp)
add_test(HDF5, test_hdf5impl)
target_link_libraries(test_hdf5impl biosignalml)

