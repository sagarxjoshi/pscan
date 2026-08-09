cmake_minimum_required (VERSION 3.5)

project (pscan)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files "${source_dir}/*.cpp")

add_executable (pscan ${source_files})

# use PkgConfig to find packages
find_package(PkgConfig REQUIRED)

# Add the include directories so the compiler finds the headers
target_include_directories(pscan PRIVATE ${LIBNET_INCLUDE_DIRS} ${PCAP_INCLUDE_DIRS})

# Add the include directories so the compiler finds the headers
target_include_directories(pscan PRIVATE ${LIBNET_INCLUDE_DIRS} ${PCAP_INCLUDE_DIRS})
target_link_libraries (pscan net pcap)

