cmake_minimum_required(VERSION 3.10)
project(GreenHouseGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


enable_testing()
add_subdirectory(test ${PROJECT_BINARY_DIR}/test)

set(SRC main FieldGenerator FieldJsonSerializer ColorGenerator)
add_executable(generator ${SRC})