cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

carpc_project( RUNTIME )



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
#                                      Code generation                                    #
#                                                                                         #
###########################################################################################
find_files_by_ext( PLANTUML_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_PLANTUML}" )
generate_plantumls( "${PLANTUML_FILES}" PROJECT_GEN_PLANTUML_FILES )



###########################################################################################
#                                                                                         #
#                                   Build source file list                                #
#                                                                                         #
###########################################################################################
find_files_by_ext( PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_CPP_SRC}" )
if( ${OS_TARGET} EQUAL ${OS_ANDROID} )
   list( REMOVE_ITEM PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/imp/carpc/oswrappers/Spinlock.cpp )
   list( REMOVE_ITEM PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/imp/carpc/oswrappers/linux/backtrace.cpp )
endif( )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_variables( )



add_library( ${PROJECT_TARGET_NAME} SHARED ${PROJECT_SOURCE_FILES} )

target_link_libraries( ${PROJECT_TARGET_NAME} ${TRACING_TARGET_NAME} )
if( ${OS_TARGET} EQUAL ${OS_LINUX} )
   target_link_libraries( ${PROJECT_TARGET_NAME} pthread )
   target_link_libraries( ${PROJECT_TARGET_NAME} rt )
endif( )
if( USE_GPB )
   target_link_libraries( ${PROJECT_TARGET_NAME} ${Protobuf_LIBRARIES} )
endif( )

add_custom_target( "${PROJECT_TARGET_NAME}_documentation" ALL
      COMMENT "cmake ${PROJECT_TARGET_NAME}_documentation"
      DEPENDS ${PROJECT_GEN_PLANTUML_FILES}
   )

add_dependencies( ${PROJECT_TARGET_NAME} "${PROJECT_TARGET_NAME}_documentation" )

install( TARGETS ${PROJECT_TARGET_NAME} DESTINATION lib )
install( DIRECTORY ${PROJECT_SOURCE_DIR}/api/ DESTINATION include )
