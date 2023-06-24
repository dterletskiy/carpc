cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

if( ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR} )
   message( FATAL_ERROR "'CMAKE_BINARY_DIR' and 'CMAKE_SOURCE_DIR' cannot have the same value." )
endif( )



include( ./build/cmake/constants.cmake )
include( ./build/cmake/functions/message.cmake )
include( ./build/cmake/functions/base.cmake )
include( ./build/cmake/configuration.cmake )
include( ./build/cmake/compiler.cmake )
include( ./build/cmake/functions/helpers.cmake )
include( ./build/cmake/functions/generators.cmake )
include( ./build/cmake/functions/debug.cmake )

carpc_project( CARPC )



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
print_environment_variables( )
print_cmake_variables( )
print_configuration( )




include( ./projects.cmake )

carpc_get_all_targets( TARGETS )
generate_dependencies_graph( ${PROJECT_GEN_DIR} ${CMAKE_PROJECT_NAME} "${TARGETS}" )
