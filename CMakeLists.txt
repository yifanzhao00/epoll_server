cmake_minimum_required(VERSION 2.5)
project(epoll_server)

#set path to dynamic library
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) 
#set debug info and c++ 11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")

#define compiled souce code
aux_source_directory(. SRC_LIST)
# produce the dynamic library epoll_server
add_library(epoll_server SHARED ${SRC_LIST})