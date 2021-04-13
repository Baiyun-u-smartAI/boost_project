cmake_minimum_required(VERSION 3.15)
project(TCP_Client_Server CXX)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "debug")
set(BOOST_ROOT "G:/mingw64/boost1.71/boost")
list(APPEND CMAKE_PREFIX_PATH ${BOOST_ROOT})
# import Boost package
FIND_PACKAGE(Boost CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

set(OpenCV_DIR "G:/mingw64/opencv/build/install")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# import OpenCV package
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})



add_executable(${PROJECT_NAME} main.cpp)
if(MINGW)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()
# set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "-fPIC -O -O1 -O2 -O3")
set(OpenCV_LIBS ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${Boost_DEBUG_LIBS}  ${OpenCV_LIBS})
# target_link_libraries(${PROJECT_NAME} z iconv)
#target_link_options(${PROJECT_NAME} PRIVATE -mwindows)

