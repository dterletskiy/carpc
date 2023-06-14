cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( TRACING )



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



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################



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



add_library( ${TARGET_NAME} SHARED ${PROJECT_SOURCE_FILES} )

if( DLT_TRACE )
   target_link_libraries( ${TARGET_NAME} dlt )
endif( )
if( ${OS_TARGET} EQUAL ${OS_ANDROID} )
   target_link_libraries( ${TARGET_NAME} log )
endif( )

install( TARGETS ${TARGET_NAME} DESTINATION lib )
install( DIRECTORY ${PROJECT_SOURCE_DIR}/api/ DESTINATION include )
if( DLT_TRACE )
   install( FILES ${PROJECT_SOURCE_DIR}/dlt.conf DESTINATION etc )
endif( )
