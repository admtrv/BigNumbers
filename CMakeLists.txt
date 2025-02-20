# CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(BigNumbers)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        main.cpp
        bignumbers.h
        tests/TestBigInteger.cpp
        tests/TestBigRational.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(GTest REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::GTest GTest::Main)

enable_testing()
add_test(NAME BigNumbersTests COMMAND BigNumbers)
