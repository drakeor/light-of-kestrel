# Look Eric! CMake! xD
# Set the minimum version of CMake to 2.8
cmake_minimum_required(VERSION 2.8)

# Our project name is Light Of Kestrel
project(lightofkestrel)

# Include our libraries
include_directories("/usr/include/")

# Add C++11 support
if (CMAKE_COMPILER_IS_GNUCXX)
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread" )
  set ( LINK_FLAGS "${LINK_FLAGS} -lpthread" )
endif ()

# Make an executable
add_executable(lightofkestrel universemanager.cpp game.cpp main.cpp)

# Link SFML Libraries.
target_link_libraries (lightofkestrel sfml-graphics sfml-system sfml-window tgui)

# Install the program if needed
install(TARGETS lightofkestrel RUNTIME DESTINATION bin)
