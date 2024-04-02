cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project (cryptolab)


add_executable(cryptolab main.c ht48.c)

target_include_directories(cryptolab
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/
)