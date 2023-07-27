cmake_minimum_required(VERSION 3.26)
project(SplayTree)

set(CMAKE_CXX_STANDARD 14)

add_library(SplayTree INTERFACE)
target_include_directories(SplayTree INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(Example1 example/Example1.cpp)
target_link_libraries(Example1 PRIVATE SplayTree)

add_executable(Example2 example/Example2.cpp)
target_link_libraries(Example2 PRIVATE SplayTree)

add_executable(Example3 example/Example3.cpp)
target_link_libraries(Example3 PRIVATE SplayTree)

add_executable(Example4 example/Example4.cpp)
target_link_libraries(Example4 PRIVATE SplayTree)
