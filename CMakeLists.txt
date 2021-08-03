cmake_minimum_required(VERSION 3.17)
project(jam)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(jam src/main.cpp)

find_package(SFML REQUIRED system window graphics network audio)

target_link_libraries(jam ${SFML_LIBRARIES})
target_link_libraries(jam sfml-graphics sfml-audio)
target_link_libraries(jam opengl32)


