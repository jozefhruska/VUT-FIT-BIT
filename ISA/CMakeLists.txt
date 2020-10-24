cmake_minimum_required(VERSION 3.10.2)
project(d6r)

add_subdirectory(classes)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads)
include(FindPCAP.cmake)

set(d6r_source_files d6r.hpp d6r.cpp)

add_executable(d6r ${d6r_source_files})
target_link_libraries(d6r ${PCAP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} classes)