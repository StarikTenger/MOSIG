cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project (cryptolab)

set(EXECUTABLES
    mac
    keyrec
    test
	colsearch
)

foreach(EXEC ${EXECUTABLES})
    add_executable(${EXEC} main_${EXEC}.c ht48.c)

	target_include_directories(${EXEC}
        PRIVATE 
            ${PROJECT_SOURCE_DIR}/
    )
endforeach()
