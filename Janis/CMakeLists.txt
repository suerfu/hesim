# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(JanisJanis)

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
# Find ROOT package
#
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Setup Geant4, ROOT include directories and compile definitions

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4, ROOT libraries
#
add_executable(Janis Janis.cc ${sources} ${headers})
target_link_libraries(Janis ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build Janis. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(Janis_SCRIPTS
  Janis.out
  /macros/HardcodeTest.mac
  /macros/JanisTest.mac
  /macros/JanisTest2.mac
  /macros/JanisTest3.mac
  /macros/localtest_original.mac
  /macros/localtest_moved.mac
  /macros/localtest_del.mac
  /macros/localtest_modified.mac
  /macros/savio10million.mac
  /macros/localtest_gamma.mac
  /macros/savio100million1.mac
  /macros/savio100million2.mac
  /macros/savio100million3.mac
  /macros/savio100million4.mac
  /macros/savio100million5.mac
  /shell/janis_savio_test3.sh
  /shell/savio10million.sh
  /shell/savio100million1.sh
  /shell/savio100million2.sh
  /shell/savio100million3.sh
  /shell/savio100million4.sh
  /shell/savio100million5.sh
  icons.mac
  gui.mac
  run.png
  init_vis.mac
  vis.mac
  /analysis/source_check.C
  /analysis/neutron_analysis.C
  )

foreach(_script ${Janis_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Janis DESTINATION bin)
