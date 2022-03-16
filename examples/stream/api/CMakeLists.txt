cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( STREAM )



###########################################################################################
#                                                                                         #
#                                      Code generation                                    #
#                                                                                         #
###########################################################################################
file( GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/*.proto )
protobuf_generate_cpp( PROTO_SRCS PROTO_HDRS ${PROTO_FILES} )

message( STATUS "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR} )
message( STATUS "PROTO_FILES = " ${PROTO_FILES} )
message( STATUS "PROTO_HDRS = " ${PROTO_HDRS} )
message( STATUS "PROTO_SRCS = " ${PROTO_SRCS} )
