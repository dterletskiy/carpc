cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

# set( CMAKE_C_COMPILER "/usr/bin/gcc" )
# set( CMAKE_CXX_COMPILER "/usr/bin/g++" )

project( RPC )



option( SYS_TRACE                "Enable framework system tracing"                              ON )
option( MSG_TRACE                "Enable application tracing"                                   ON )
option( COLORED_TRACE            "Enable colored tracing for console"                           ON )
option( DLT_TRACE                "Enable dlt tracing"                                           ON )
option( MEMORY_HOOK              "Enable memory allocator hooks"                                OFF )
option( INSTRUMENTAL             "Enable instrumental functionality"                            OFF )



###########################################################################################
#                                                                                         #
#                                Setup common configuration                               #
#                                                                                         #
###########################################################################################
# set(CMAKE_GENERATOR "Sublime Text 2 - Unix Makefiles")
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED True )
set( CMAKE_VERBOSE_MAKEFILE on )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
# link_directories( "/usr/local/lib/" )



###########################################################################################
#                                                                                         #
#                                   Compile definitions                                   #
#                                                                                         #
###########################################################################################
add_definitions( -Wall )
add_definitions( -Wextra )
add_definitions( -Wno-ignored-qualifiers )
add_definitions( -Wno-unused-but-set-variable )
add_definitions( -Wno-unused-variable )
add_definitions( -Wno-unused-parameter )
add_definitions( -Wno-unused-function )
add_definitions( -Wno-comment )
if( INSTRUMENTAL )
   add_definitions( -O0 )
   add_definitions( -g )
   add_definitions( -finstrument-functions )
else( )
   add_definitions( -Ofast )
endif( )

add_compile_definitions( OS_ANDROID=0 )
add_compile_definitions( OS_LINUX=1 )
add_compile_definitions( OS=OS_LINUX )
if( SYS_TRACE )
   add_compile_definitions( SYS_TRACE )
endif( )
if( MSG_TRACE )
   add_compile_definitions( MSG_TRACE )
endif( )
if( COLORED_TRACE )
   add_compile_definitions( COLORED_TRACE )
endif( )
if( DLT_TRACE )
   add_compile_definitions( USE_DLT )
endif( )
if( MEMORY_HOOK )
   add_compile_definitions( USE_MEMORY_HOOK )
endif( )



###########################################################################################
#                                                                                         #
#                                   Build source file list                                #
#                                                                                         #
###########################################################################################



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
message( NOTICE "-----------------------------------------------------------------------" )
message( NOTICE "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR} )
message( NOTICE "PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR} )
message( NOTICE "CMAKE_CURRENT_LIST_DIR = " ${CMAKE_CURRENT_LIST_DIR} )
message( NOTICE "CMAKE_GENERATOR = " ${CMAKE_GENERATOR} )
message( NOTICE "RPC_SOURCE_DIR = " ${RPC_SOURCE_DIR} )
message( NOTICE "RPC_BINARY_DIR = " ${RPC_BINARY_DIR} )
message( NOTICE "CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX} )
message( NOTICE "-----------------------------------------------------------------------" )



add_subdirectory( tracing )
add_subdirectory( framework )
add_subdirectory( hooks )
add_subdirectory( instrumental )
add_subdirectory( servicebrocker )
add_subdirectory( application )
add_subdirectory( controller )
add_subdirectory( core )
add_subdirectory( hmi )
add_subdirectory( experimental )

install( FILES ${PROJECT_SOURCE_DIR}/.configuration/dlt.conf DESTINATION etc )
