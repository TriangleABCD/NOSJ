cmake_minimum_required(VERSION 3.16)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)

project(main LANGUAGES CXX)

include_directories(./include)

aux_source_directory(./src SRC)


add_executable(main ${SRC})
