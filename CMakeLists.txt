#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.24)
project(DSPS)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find NEST package
#
find_package(NEST REQUIRED)
include_directories(${NEST_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and NEST libraries
#
add_compile_options(-g)
add_executable(DSPS DSPS.cc ${sources} ${headers})
target_link_libraries(DSPS ${Geant4_LIBRARIES} NEST::NESTG4)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build DSPS. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(DSPS_SCRIPTS
    macros/parameters_detector.mac
    macros/parameters_output.mac
    macros/gui.mac
    macros/vis.mac
   )

foreach(_script ${DSPS_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DSPS DESTINATION bin)