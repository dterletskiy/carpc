cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( HOOKS )



###########################################################################################
#                                                                                         #
#                                Setup common configuration                               #
#                                                                                         #
###########################################################################################
get_filename_component( TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
string( REPLACE " " "_" TARGET_NAME ${TARGET_NAME} )
message( NOTICE "TARGET_NAME = " ${TARGET_NAME} )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${TRACING_SOURCE_DIR} )
include_directories( ${FRAMEWORK_SOURCE_DIR} )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( ${TRACING_BINARY_DIR} )
link_directories( ${FRAMEWORK_BINARY_DIR} )



###########################################################################################
#                                                                                         #
#                                   Compile definitions                                   #
#                                                                                         #
###########################################################################################



###########################################################################################
#                                                                                         #
#                                   Build source file list                                #
#                                                                                         #
###########################################################################################
file( GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
message( NOTICE "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR} )
message( NOTICE "PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR} )
message( NOTICE "CMAKE_CURRENT_LIST_DIR = " ${CMAKE_CURRENT_LIST_DIR} )
message( STATUS "SOURCE_FILES = " ${SOURCE_FILES} )



add_library( ${TARGET_NAME} SHARED ${SOURCE_FILES} )

target_link_libraries( ${TARGET_NAME} tracing )
target_link_libraries( ${TARGET_NAME} framework )

install( TARGETS ${TARGET_NAME} DESTINATION lib )
