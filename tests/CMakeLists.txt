add_executable(test_bsml bsml.cpp)
target_link_libraries(test_bsml bsml)
add_test(BSML, test_bsml)

add_executable(test_hdf5 hdf5.cpp)
target_link_libraries(test_hdf5 bsml)
add_test(HDF5, test_hdf5)

add_executable(test_hdf5impl hdf5impl.cpp)
target_link_libraries(test_hdf5impl bsml)
add_test(HDF5, test_hdf5impl)

