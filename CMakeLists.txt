cmake_minimum_required(VERSION 2.8)

# Set up project
set(PROJ_NAME controllershell)
project(${PROJ_NAME} CXX)

# Set C++ 11
add_definitions(-std=c++11)

# Include directory
include_directories(include)

# Add all project sources
file(GLOB SOURCES 
	*.cpp
	*.h
)

add_executable(${PROJ_NAME} ${SOURCES})

# Link to specified libraries
target_link_libraries(${PROJ_NAME}
	sdl2
	sdl2_ttf
	sdl2_mixer
	inireader
)