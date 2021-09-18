cmake_minimum_required(VERSION 3.20)
project(untitled)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp)
LINK_DIRECTORIES("C:\\msys64\\mingw64\\lib")
target_link_libraries(${PROJECT_NAME} boost_program_options-mt ws2_32)
