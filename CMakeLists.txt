# Look Eric! CMake! xD
# Set the minimum version of CMake to 2.8
cmake_minimum_required(VERSION 2.8)

# To build it needs the following (Tested on Ubuntu 12.04)

# sudo apt-get install libx11-dev libglui-dev libxi-dev libxmu-dev
# SFML 2.1 Libraries (http://www.sfml-dev.org/download/sfml/2.1/)
# Glew 1.7.0 (http://sourceforge.net/projects/glew/files/glew/1.7.0/)
# One should be able to build the project and it's dependancies at this point.

# Our project name is Light Of Kestrel
project(lightofkestrel)

# Include our libraries
include_directories("/usr/include/")
include_directories("universe/")
include_directories("gui/")
include_directories("./")

# Add C++11 support
if (CMAKE_COMPILER_IS_GNUCXX)
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread -Wall" )
  set ( LINK_FLAGS "${LINK_FLAGS} -lpthread" )
endif ()

# Make an executable
add_executable(lightofkestrel ai/shipai.cpp ai/relationshipmanager.cpp ai/turretai.cpp ai/baseai.cpp controllers/shipinterfacecontroller.cpp components/vectorthruster.cpp components/armourplating.cpp universe/missilefactory.cpp components/missilebay.cpp gui/dropdown.cpp components/entitycomponent.cpp universe/entityfactory.cpp controllers/worldcontroller.cpp controllers/myplayercontroller.cpp controllers/basecontroller.cpp controllers/controllermanager.cpp gui/slider.cpp gui/button.cpp events/eventhandler.cpp events/eventlistener.cpp camera.cpp gui/textlabel.cpp gui/basecontrol.cpp gui/guimanager.cpp assetmanager.cpp inputmanager.cpp universe/entity.cpp universe/galaxy.cpp universe/universemanager.cpp game.cpp main.cpp settings.cpp)

# Link SFML Libraries.
if(WIN32)
  target_link_libraries(lightofkestrel sfml-graphics-d sfml-system-d sfml-window-d)
else()
  target_link_libraries (lightofkestrel sfml-graphics sfml-system sfml-window)
endif()

# Install the program if needed
install(TARGETS lightofkestrel RUNTIME DESTINATION bin)
#INSTALL(TARGETS lightofkestrel RUNTIME DESTINATION ${lightofkestrel_SOURCE_DIR}/deploy)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/deploy)

add_subdirectory(gui)
add_subdirectory(universe)
add_subdirectory(controllers)
add_subdirectory(events)
add_subdirectory(components)
add_subdirectory(ai)