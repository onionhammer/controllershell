cmake_minimum_required(VERSION 2.8)

project (ControllerShell CXX)

ADD_DEFINITIONS(
    -std=c++11
)

file(GLOB SOURCES *.cpp)

add_executable(controllershell ${SOURCES})
target_link_libraries (controllershell sdl2 sdl2_ttf sdl2_mixer inireader)