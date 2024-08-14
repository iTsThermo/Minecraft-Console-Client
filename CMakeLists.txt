cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(MyProject main.cpp
        read_write_varint_varlong.cpp
        read_write_varint_varlong.h
        client.cpp
        client.h)

target_link_libraries(MyProject ws2_32)
