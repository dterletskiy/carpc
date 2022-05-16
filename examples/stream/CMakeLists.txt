cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( STREAM )



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
include_directories( ${PROJECT_GEN_DIR} )



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
#                                      Code generation                                    #
#                                                                                         #
###########################################################################################
file( GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/*.proto )
# protobuf_generate_cpp( PROTO_SRCS PROTO_HDRS ${PROTO_FILES} )

list( APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR} )
foreach( PROTO_FILE ${PROTO_FILES} )
   execute_process(
         COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROJECT_GEN_DIR} ${PROTO_FILE}
      )
endforeach( )



###########################################################################################
#                                                                                         #
#                                   Build source file list                                #
#                                                                                         #
###########################################################################################
file( GLOB_RECURSE PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp )
file( GLOB_RECURSE PROJECT_GEN_FILES ${PROJECT_GEN_DIR}/*.cc )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_project_files( )



add_executable( ${TARGET_NAME} ${PROJECT_SOURCE_FILES} ${PROJECT_GEN_FILES} ${PROTO_SRCS} ${PROTO_HDRS} )

target_link_libraries( ${TARGET_NAME} tracing )
target_link_libraries( ${TARGET_NAME} framework )
if( MEMORY_HOOK )
   target_link_libraries( ${TARGET_NAME} hooks )
endif( )
if( INSTRUMENTAL )
   target_link_libraries( ${TARGET_NAME} instrumental )
endif( )

install( TARGETS ${TARGET_NAME} DESTINATION bin )
install( FILES ${PROJECT_SOURCE_DIR}/${TARGET_NAME}.cfg DESTINATION etc )
