cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( FRAMEWORK )



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
file( GLOB_RECURSE PLANTUML_FILES ${PROJECT_SOURCE_DIR}/*.plantuml )
generate_plantumls( "${PLANTUML_FILES}" PROJECT_GEN_PLANTUML_FILES )



###########################################################################################
#                                                                                         #
#                                   Build source file list                                #
#                                                                                         #
###########################################################################################
file( GLOB_RECURSE PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp )
if( ${OS_TARGET} EQUAL ${OS_ANDROID} )
   list( REMOVE_ITEM PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/imp/carpc/oswrappers/Spinlock.cpp )
   list( REMOVE_ITEM PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/imp/carpc/oswrappers/linux/backtrace.cpp )
endif( )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_project_files( )



add_library( ${TARGET_NAME} SHARED ${PROJECT_SOURCE_FILES} )

target_link_libraries( ${TARGET_NAME} tracing )
if( ${OS_TARGET} EQUAL ${OS_LINUX} )
   target_link_libraries( ${TARGET_NAME} pthread )
   target_link_libraries( ${TARGET_NAME} rt )
endif( )
if( USE_GPB )
   target_link_libraries( ${TARGET_NAME} ${Protobuf_LIBRARIES} )
endif( )

add_custom_target( "${TARGET_NAME}_documentation" ALL
      COMMENT "cmake ${TARGET_NAME}_documentation"
      DEPENDS ${PROJECT_GEN_PLANTUML_FILES}
   )

install( TARGETS ${TARGET_NAME} DESTINATION lib )
install( DIRECTORY ${PROJECT_SOURCE_DIR}/api/ DESTINATION include )
