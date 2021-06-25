#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(genericBiasing)

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
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
set(CMAKE_INSTALL_PREFIX build_${GIT_COMMIT_HASH})



#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(genericBiasing genericBiasing.cc ${sources} ${headers})
target_link_libraries(genericBiasing ${Geant4_LIBRARIES})

if (UNIX AND NOT APPLE)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/macros/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/macros/)
else()
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/macros/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/macros/)
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(MYGEANT4TEMPLATE_SCRIPTS)

foreach(_script ${MYGEANT4TEMPLATE_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to directory CMAKE_INSTALL_PREFIX
#
install(TARGETS genericBiasing DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_PREFIX})
