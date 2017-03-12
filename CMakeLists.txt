cmake_minimum_required(VERSION 3.7)

set(CMAKE_CC_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
set(CMAKE_CC_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include_directories(include)

aux_source_directory(lib    lib_sources)
aux_source_directory(driver driver_sources)
aux_source_directory(test   test_sources)

find_package(GTest REQUIRED)

add_library(c_expr SHARED ${lib_sources})

add_executable(c_expr_driver ${driver_sources})
target_link_libraries(c_expr_driver c_expr m)

add_executable(c_expr_test ${test_sources})
target_link_libraries(c_expr_test c_expr GTest::GTest GTest::Main)
