cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( APPLICATION )



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
include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${PROJECT_GEN_DIR} )
include_directories( ${CARPC_INCLUDE_DIR} )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( ${CARPC_LIB_DIR} )



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
find_files_by_ext( ADL_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_ADL}" )
generate_xdls( "${ADL_FILES}" PROJECT_GEN_ADL_FILES )

find_files_by_ext( IDL_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_IDL}" )
generate_xdls( "${IDL_FILES}" PROJECT_GEN_IDL_FILES )

find_files_by_ext( DDL_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_DDL}" )
generate_xdls( "${DDL_FILES}" PROJECT_GEN_DDL_FILES )

find_files_by_ext( PROTO_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_PROTO}" )
generate_gpbs( "${PROTO_FILES}" PROJECT_GEN_PROTO_FILES )

find_files_by_ext( PLANTUML_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_PLANTUML}" )
generate_plantumls( "${PLANTUML_FILES}" PROJECT_GEN_PLANTUML_FILES )



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



add_executable(
   ${TARGET_NAME}
   ${PROJECT_SOURCE_FILES}
   ${PROJECT_GEN_ADL_FILES}
   ${PROJECT_GEN_IDL_FILES}
   ${PROJECT_GEN_DDL_FILES}
   ${PROJECT_GEN_PROTO_FILES}
)

target_link_libraries( ${TARGET_NAME} tracing )
target_link_libraries( ${TARGET_NAME} runtime )
if( MEMORY_HOOK )
   target_link_libraries( ${TARGET_NAME} hooks )
endif( )
if( INSTRUMENTAL )
   target_link_libraries( ${TARGET_NAME} instrumental )
endif( )

if( DLT_TRACE )
   target_link_libraries( ${TARGET_NAME} dlt )
endif( )
if( ${OS_TARGET} EQUAL ${OS_ANDROID} )
   target_link_libraries( ${TARGET_NAME} log )
endif( )

if( USE_GPB )
   target_link_libraries( ${TARGET_NAME} ${Protobuf_LIBRARIES} )
endif( )

add_custom_target( "${TARGET_NAME}_documentation" ALL
      COMMENT "cmake ${TARGET_NAME}_documentation"
      DEPENDS ${PROJECT_GEN_PLANTUML_FILES}
   )

install( TARGETS ${TARGET_NAME} DESTINATION bin )
install( FILES ${PROJECT_SOURCE_DIR}/${TARGET_NAME}.cfg DESTINATION etc OPTIONAL )
