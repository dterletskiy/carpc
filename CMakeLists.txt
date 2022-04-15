cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( RPC )

option( SYS_TRACE                "Enable framework system tracing"                              ON )
option( MSG_TRACE                "Enable application tracing"                                   ON )
option( COLORED_TRACE            "Enable colored tracing for console"                           ON )
option( DLT_TRACE                "Enable dlt tracing"                                           OFF )
option( MEMORY_HOOK              "Enable memory allocator hooks"                                OFF )
option( INSTRUMENTAL             "Enable instrumental functionality"                            OFF )
option( USE_DEBUG                "Enable debug information"                                     OFF )
option( USE_GPB                  "Enable goolgle protobuf"                                      ON )
option( USE_RTTI                 "Enable RTTI"                                                  ON )



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
# https://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
   message( NOTICE "compiler: Clang" )
   add_definitions( -Wall )
   add_definitions( -Wextra )
   add_definitions( -Wno-ignored-qualifiers )
   add_definitions( -Wno-unused-const-variable )
   add_definitions( -Wno-unused-variable )
   add_definitions( -Wno-unused-parameter )
   add_definitions( -Wno-unused-function )
   add_definitions( -Wno-unused-result )
   add_definitions( -Wno-unused-private-field )
   add_definitions( -Wno-overloaded-virtual )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
   message( NOTICE "compiler: GCC" )
   add_definitions( -Wall )
   add_definitions( -Wextra )
   add_definitions( -Wno-ignored-qualifiers )
   add_definitions( -Wno-unused-but-set-variable )
   add_definitions( -Wno-unused-variable )
   add_definitions( -Wno-unused-parameter )
   add_definitions( -Wno-unused-function )
   add_definitions( -Wno-unused-result )
   add_definitions( -Wno-comment )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "Intel" )
   message( NOTICE "compiler: Intel C++" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
   message( NOTICE "compiler: Visual Studio C++" )
endif( )



if( INSTRUMENTAL )
   add_definitions( -O0 )
   add_definitions( -g )
   add_definitions( -finstrument-functions )
else( )
   add_definitions( -Ofast )
endif( )
if( USE_DEBUG OR INSTRUMENTAL )
   add_definitions( -g )
endif( )
if( USE_RTTI )
   add_definitions( -frtti )
else( )
   add_definitions( -fno-rtti )
endif( )

add_compile_definitions( OS_ANDROID=${OS_ANDROID} )
add_compile_definitions( OS_LINUX=${OS_LINUX} )
add_compile_definitions( OS_TARGET=${OS_TARGET} )
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
if( USE_GPB )
   find_package( Protobuf REQUIRED )
   add_compile_definitions( USE_GPB )
endif( )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
message( NOTICE "-----------------------------------------------------------------------" )
message( NOTICE "OS_ANDROID = " ${OS_ANDROID} )
message( NOTICE "OS_LINUX = " ${OS_LINUX} )
message( NOTICE "OS_TARGET = " ${OS_TARGET} )
message( NOTICE "-----------------------------------------------------------------------" )
message( NOTICE "CMAKE_C_COMPILER = " ${CMAKE_C_COMPILER} )
message( NOTICE "CMAKE_CXX_COMPILER = " ${CMAKE_CXX_COMPILER} )
message( NOTICE "-----------------------------------------------------------------------" )
message( NOTICE "CMAKE_HOME_DIRECTORY = " ${CMAKE_HOME_DIRECTORY} )
message( NOTICE "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR} )
message( NOTICE "PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR} )
message( NOTICE "CMAKE_CURRENT_LIST_DIR = " ${CMAKE_CURRENT_LIST_DIR} )
message( NOTICE "CMAKE_GENERATOR = " ${CMAKE_GENERATOR} )
message( NOTICE "RPC_SOURCE_DIR = " ${RPC_SOURCE_DIR} )
message( NOTICE "RPC_BINARY_DIR = " ${RPC_BINARY_DIR} )
message( NOTICE "CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX} )
message( NOTICE "ROOT_GEN_DIR = " ${ROOT_GEN_DIR} )
message( NOTICE "-----------------------------------------------------------------------" )
message( NOTICE "TOOLCHAIN: $ENV{TOOLCHAIN}" )
message( NOTICE "SYSROOT: $ENV{SYSROOT}" )
message( NOTICE "ARCH: $ENV{ARCH}" )
message( NOTICE "OS: $ENV{OS}" )
message( NOTICE "API: $ENV{API}" )
message( NOTICE "TARGET: $ENV{TARGET}" )
message( NOTICE "SESSION: $ENV{SESSION}" )
message( NOTICE "AR: $ENV{AR}" )
message( NOTICE "AS: $ENV{AS}" )
message( NOTICE "CC: $ENV{CC}" )
message( NOTICE "CXX: $ENV{CXX}" )
message( NOTICE "LD: $ENV{LD}" )
message( NOTICE "RANLIB: $ENV{RANLIB}" )
message( NOTICE "STRIP: $ENV{STRIP}" )
message( NOTICE "-----------------------------------------------------------------------" )
message( NOTICE "ALL VARIABLES:" )
# execute_process( COMMAND "${CMAKE_COMMAND}" "-E" "environment" )

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
message( NOTICE "-----------------------------------------------------------------------" )



add_subdirectory( tracing )
add_subdirectory( framework )
add_subdirectory( servicebrocker )
add_subdirectory( service )
# add_subdirectory( hooks )
# add_subdirectory( instrumental )
add_subdirectory( examples )
