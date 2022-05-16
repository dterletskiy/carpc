cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( INSTRUMENTAL )



###########################################################################################
#                                                                                         #
#                                Setup common configuration                               #
#                                                                                         #
###########################################################################################
setup_target_name( )
setup_gen_dir( )



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
file( GLOB_RECURSE PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_project_files( )



add_library( ${TARGET_NAME} SHARED ${PROJECT_SOURCE_FILES} )

target_link_libraries( ${TARGET_NAME} dl )
target_link_libraries( ${TARGET_NAME} tracing )

install( TARGETS ${TARGET_NAME} DESTINATION lib )
