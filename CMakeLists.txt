cmake_minimum_required(VERSION 3.17)
project(jam)
project(rama2)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(jam src/Ramazan.cpp)
add_executable(rama2 src/main.cpp)

find_package(SFML REQUIRED system window graphics network audio)


target_link_libraries(jam ${SFML_LIBRARIES})
target_link_libraries(jam sfml-graphics sfml-audio)
target_link_libraries(jam opengl32)

target_link_libraries(rama2 ${SFML_LIBRARIES})
target_link_libraries(rama2 sfml-graphics sfml-audio)
target_link_libraries(rama2 opengl32)

