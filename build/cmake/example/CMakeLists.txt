cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( APPLICATION )



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
file( GLOB_RECURSE ADL_FILES ${PROJECT_SOURCE_DIR}/*.adl )
generate_xdls( "${ADL_FILES}" PROJECT_GEN_ADL_FILES )

file( GLOB_RECURSE IDL_FILES ${PROJECT_SOURCE_DIR}/*.idl )
generate_xdls( "${IDL_FILES}" PROJECT_GEN_IDL_FILES )

file( GLOB_RECURSE DDL_FILES ${PROJECT_SOURCE_DIR}/*.ddl )
generate_xdls( "${DDL_FILES}" PROJECT_GEN_DDL_FILES )

file( GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/*.proto )
generate_gpbs( "${PROTO_FILES}" PROJECT_GEN_PROTO_FILES )

file( GLOB_RECURSE PLANTUML_FILES ${PROJECT_SOURCE_DIR}/*.plantuml )
generate_plantumls( "${PLANTUML_FILES}" PROJECT_GEN_PLANTUML_FILES )



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
