# CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(BigNumbers)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp tests.cpp bignumbers.h)

add_executable(${PROJECT_NAME} ${SOURCES})
