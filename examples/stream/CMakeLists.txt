cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( STREAM )



###########################################################################################
#                                                                                         #
#                                Setup common configuration                               #
#                                                                                         #
###########################################################################################
get_filename_component( TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
string( REPLACE " " "_" TARGET_NAME ${TARGET_NAME} )
message( NOTICE "TARGET_NAME = " ${TARGET_NAME} )


string( REGEX REPLACE "${CMAKE_HOME_DIRECTORY}" "${ROOT_GEN_DIR}" PROJECT_GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
file( MAKE_DIRECTORY ${PROJECT_GEN_DIR} )



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
file( GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp )
file( GLOB_RECURSE GEN_FILES ${PROJECT_GEN_DIR}/*.cc )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
message( NOTICE "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR} )
message( NOTICE "PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR} )
message( NOTICE "PROJECT_GEN_DIR = " ${PROJECT_GEN_DIR} )
message( NOTICE "CMAKE_CURRENT_LIST_DIR = " ${CMAKE_CURRENT_LIST_DIR} )
message( STATUS "SOURCE_FILES = " ${SOURCE_FILES} )
message( STATUS "GEN_FILES = " ${GEN_FILES} )
message( STATUS "PROTO_FILES = " ${PROTO_FILES} )
message( STATUS "PROTO_HDRS = " ${PROTO_HDRS} )
message( STATUS "PROTO_SRCS = " ${PROTO_SRCS} )



add_executable( ${TARGET_NAME} ${SOURCE_FILES} ${GEN_FILES} ${PROTO_SRCS} ${PROTO_HDRS} )

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
