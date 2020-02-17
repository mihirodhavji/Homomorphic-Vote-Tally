cmake_minimum_required(VERSION 3.12)
project(SEALDemo VERSION 1.0)
add_executable(set set.cpp)
find_package(SEAL) 
target_link_libraries(set SEAL::seal)

