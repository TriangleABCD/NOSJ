cmake_minimum_required(VERSION 3.16)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 20)

project(main LANGUAGES CXX)

add_compile_options(-Wall)

include_directories(./include)

aux_source_directory(./src SRC)


add_executable(main ${SRC})
