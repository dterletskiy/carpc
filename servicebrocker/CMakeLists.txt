cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( SERVICEBROCKER )



###########################################################################################
#                                                                                         #
#                                Setup common configuration                               #
#                                                                                         #
###########################################################################################
setup_project( TARGET_NAME )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( ${PROJECT_SOURCE_DIR}/api )
include_directories( ${TRACING_SOURCE_DIR}/api )
include_directories( ${FRAMEWORK_SOURCE_DIR}/api )



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
find_files_by_ext( PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_CPP_SRC}" )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_project_files( )



add_executable( ${TARGET_NAME} ${PROJECT_SOURCE_FILES} )

target_link_libraries( ${TARGET_NAME} tracing )
target_link_libraries( ${TARGET_NAME} runtime )
if( MEMORY_HOOK )
   target_link_libraries( ${TARGET_NAME} hooks )
endif( )
if( INSTRUMENTAL )
   target_link_libraries( ${TARGET_NAME} instrumental )
endif( )

install( TARGETS ${TARGET_NAME} DESTINATION bin )
install( FILES ${PROJECT_SOURCE_DIR}/${TARGET_NAME}.cfg DESTINATION etc )
