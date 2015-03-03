add_executable(test_bsml bsml.cpp)
target_link_libraries(test_bsml bsml)
add_test(BSML, test_bsml)
