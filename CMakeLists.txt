cmake_minimum_required(VERSION 3.17)
project(jam)

set(CMAKE_CXX_STANDARD 20)

add_executable(jam main.cpp src/button.c include/button.h include/menu.h)

find_package(SFML REQUIRED system window graphics network audio)

include_directories(include)
target_link_libraries(jam ${SFML_LIBRARIES})
target_link_libraries(jam sfml-graphics sfml-audio)
target_link_libraries(jam opengl32)
#target_link_libraries(jam)
