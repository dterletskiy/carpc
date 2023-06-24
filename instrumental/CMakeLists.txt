cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

carpc_project( INSTRUMENTAL )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( ${PROJECT_SOURCE_DIR}/api )
include_directories( ${TRACING_SOURCE_DIR}/api )



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
find_files_by_ext( PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_CPP_SRC}" )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_variables( )



add_library( ${PROJECT_TARGET_NAME} SHARED ${PROJECT_SOURCE_FILES} )

target_link_libraries( ${PROJECT_TARGET_NAME} dl )
target_link_libraries( ${PROJECT_TARGET_NAME} ${TRACING_TARGET_NAME} )

install( TARGETS ${PROJECT_TARGET_NAME} DESTINATION lib )
