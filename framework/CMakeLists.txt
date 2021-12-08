cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( FRAMEWORK )



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



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( ${TRACING_BINARY_DIR} )



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
# list( REMOVE_ITEM SOURCE_FILES ${PROJECT_SOURCE_DIR}/imp/sys/oswrappers/Spinlock.cpp )



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
if( ${OS} EQUAL ${OS_LINUX} )
   target_link_libraries( ${TARGET_NAME} pthread )
endif( )

install( TARGETS ${TARGET_NAME} DESTINATION lib )
install( DIRECTORY ${PROJECT_SOURCE_DIR}/api DESTINATION . )
