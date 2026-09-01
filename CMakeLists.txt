cmake_minimum_required (VERSION 3.5)

project (pscan)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++23")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")
set (include_dir "${PROJECT_SOURCE_DIR}/include/")

set(source_files
	"${source_dir}/main.cpp"
	"${source_dir}/output.cpp"
	"${source_dir}/capture.cpp"
	"${source_dir}/write_pckts.cpp"
	"${source_dir}/proc_pckts.cpp"
	"${source_dir}/cidr.cpp"
)

add_executable (pscan ${source_files})

# use PkgConfig to find packages
find_package(PkgConfig REQUIRED)

# Add the include directories so the compiler finds the headers
target_include_directories(pscan PRIVATE ${include_dir})

target_link_libraries (pscan net pcap)

