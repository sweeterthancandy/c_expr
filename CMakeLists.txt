cmake_minimum_required(VERSION 3.7)

set(CMAKE_CC_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
set(CMAKE_CC_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(c_expr c_expr.c)
target_link_libraries(c_expr m)

#target_link_libraries(ss_test GTest::GTest GTest::Main)
#target_link_libraries(ss_test ss_lib)
