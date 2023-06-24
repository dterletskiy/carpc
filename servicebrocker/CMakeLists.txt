cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

carpc_project( SERVICEBROCKER )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( ${PROJECT_SOURCE_DIR}/api )
include_directories( ${TRACING_SOURCE_DIR}/api )
include_directories( ${RUNTIME_SOURCE_DIR}/api )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( ${TRACING_BINARY_DIR} )
link_directories( ${RUNTIME_BINARY_DIR} )



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



add_executable( ${PROJECT_TARGET_NAME} ${PROJECT_SOURCE_FILES} )

target_link_libraries( ${PROJECT_TARGET_NAME} ${TRACING_TARGET_NAME} )
target_link_libraries( ${PROJECT_TARGET_NAME} ${RUNTIME_TARGET_NAME} )
if( MEMORY_HOOK )
   target_link_libraries( ${PROJECT_TARGET_NAME} ${HOOKS_TARGET_NAME} )
endif( )
if( INSTRUMENTAL )
   target_link_libraries( ${PROJECT_TARGET_NAME} ${INSTRUMENTAL_TARGET_NAME} )
endif( )

install( TARGETS ${PROJECT_TARGET_NAME} DESTINATION bin )
install( FILES ${PROJECT_SOURCE_DIR}/${PROJECT_TARGET_NAME}.cfg DESTINATION etc )
