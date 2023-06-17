cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

if( ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR} )
   message( FATAL_ERROR "'CMAKE_BINARY_DIR' and 'CMAKE_SOURCE_DIR' cannot have the same value." )
endif( )

project( CARPC )

include( ./build/cmake/constants.cmake )
include( ./build/cmake/functions/message.cmake )
include( ./build/cmake/functions/helpers.cmake )
include( ./build/cmake/configuration.cmake )
include( ./build/cmake/compiler.cmake )
include( ./build/cmake/debug.cmake )
include( ./build/cmake/functions/setup.cmake )
include( ./build/cmake/functions/generators.cmake )



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
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
# print_project_files( )
# print_toolchain_config( )
# print_environment_variables( )
# print_cmake_variables( )
print_configuration( )



include( ./projects.cmake )
