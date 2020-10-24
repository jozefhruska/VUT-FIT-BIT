set(classes_source_files
  App.hpp
  Sniffer.hpp
  Relay.hpp
  App.cpp
  Sniffer.cpp
  Relay.cpp
)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

include(../FindPCAP.cmake)
add_library(classes ${classes_source_files})
target_link_libraries(classes ${PCAP_LIBRARY})