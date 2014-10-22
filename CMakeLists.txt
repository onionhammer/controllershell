cmake_minimum_required(VERSION 2.8)

# Set up project
set(PROJ_NAME controllershell)
project(${PROJ_NAME} CXX)

# Set C++ 11
add_definitions(-std=c++11 -stdlib=libc++)

# Include directory
include_directories(include)

# Add all project sources
file(GLOB SOURCES
	*.cpp
	*.h
)

add_executable(
    ${PROJ_NAME}
    ${SOURCES}
    settings.ini
)

# Link to specified libraries
find_library(SDL2_LIBRARY SDL2 HINTS lib)
find_library(SDL2_TTF_LIBRARY SDL2_ttf HINTS lib)
find_library(SDL2_MIXER_LIBRARY SDL2_mixer HINTS lib)
find_library(INIREADER_LIBRARY INIReader HINTS lib)

target_link_libraries(${PROJ_NAME}
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_MIXER_LIBRARY}
	${INIREADER_LIBRARY}
)