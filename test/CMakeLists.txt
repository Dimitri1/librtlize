cmake_minimum_required(VERSION 3.0)
project(tests)

find_package(SystemCLanguage CONFIG REQUIRED)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

macro(make_test name src)
add_executable(${name} ${src})
target_link_libraries( ${name} SystemC::systemc)
endmacro()



make_test(minimal minimal.cpp)
