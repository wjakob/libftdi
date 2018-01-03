find_package(Boost COMPONENTS unit_test_framework REQUIRED)

enable_testing()

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

set(cpp_tests basic.cpp baudrate.cpp)

add_executable(test_libftdi1 ${cpp_tests})
target_link_libraries(test_libftdi1 ftdi1 ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(test_libftdi1 test_libftdi1)

# Add custom target so we run easily run "make check"
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_libftdi1)
