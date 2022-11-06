cmake_minimum_required(VERSION 3.20)

project(Matrices LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# cmake -DCMAKE_BUILD_TYPE=Debug ..
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")                # common
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g -DDEBUG")                   # debug only
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG -march=native")  # release only

add_subdirectory(src)
