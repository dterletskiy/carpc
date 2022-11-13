cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( CARPC )

include( ./CMakeLists-compiler_definitions.txt )
include( ./build/cmake/CMakeLists-message.txt )
include( ./build/cmake/CMakeLists-debug_print.txt )
include( ./build/cmake/CMakeLists-setup_macros.txt )
include( ./build/cmake/CMakeLists-generators.txt )



###########################################################################################
#                                                                                         #
#                                Setup common configuration                               #
#                                                                                         #
###########################################################################################
# set(CMAKE_GENERATOR "Sublime Text 2 - Unix Makefiles")
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED True )
set( CMAKE_VERBOSE_MAKEFILE on )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( "$ENV{HOME}/.local/include" )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( "/usr/local/lib/" )
link_directories( "$ENV{HOME}/.local/lib" )



###########################################################################################
#                                                                                         #
#                                        Thirdparty                                       #
#                                                                                         #
###########################################################################################
set( ANTLR4_JAR ${ROOT_SOURCE_DIR}/thirdparty/antlr/antlr-4.10.1-complete.jar )
set( PLANTUML_JAR ${ROOT_SOURCE_DIR}/thirdparty/plantuml/plantuml-1.2021.14.jar )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_project_files( )
print_toolchain_config( )
print_environment_variables( )
print_cmake_variables( )



include( ./CMakeLists-projects.txt )
