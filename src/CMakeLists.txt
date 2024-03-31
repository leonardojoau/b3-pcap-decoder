cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(b3_pcap_decoder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(decoder decoder.cpp)

target_link_libraries(decoder pcap)
