cmake_minimum_required(VERSION 3.12)
project(SEALDemo VERSION 1.0)
add_executable(tallyofficial tallyofficial.cpp)
find_package(SEAL) 
target_link_libraries(tallyofficial SEAL::seal)

