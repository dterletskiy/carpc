cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( CORE )



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
include_directories( ${FRAMEWORK_SOURCE_DIR} )
include_directories( ${SERVICE_SOURCE_DIR} )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( ${TRACING_BINARY_DIR} )
link_directories( ${FRAMEWORK_BINARY_DIR} )
link_directories( ${SERVICE_BINARY_DIR} )



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



add_executable( ${TARGET_NAME} ${PROJECT_SOURCE_FILES} )

target_link_libraries( ${TARGET_NAME} tracing )
target_link_libraries( ${TARGET_NAME} framework )
target_link_libraries( ${TARGET_NAME} service )
if( MEMORY_HOOK )
   target_link_libraries( ${TARGET_NAME} hooks )
endif( )
if( INSTRUMENTAL )
   target_link_libraries( ${TARGET_NAME} instrumental )
endif( )

install( TARGETS ${TARGET_NAME} DESTINATION bin )
install( FILES ${PROJECT_SOURCE_DIR}/${TARGET_NAME}.cfg DESTINATION etc )
