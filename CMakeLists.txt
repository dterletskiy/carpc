cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( CARPC )

option( SYS_TRACE                "Enable framework system tracing"                              ON )
option( MSG_TRACE                "Enable application tracing"                                   ON )
option( COLORED_TRACE            "Enable colored tracing for console"                           ON )
option( DLT_TRACE                "Enable dlt tracing"                                           OFF )
option( MEMORY_HOOK              "Enable memory allocator hooks"                                OFF )
option( INSTRUMENTAL             "Enable instrumental functionality"                            OFF )
option( USE_DEBUG                "Enable debug information"                                     OFF )
option( USE_GPB                  "Enable goolgle protobuf"                                      ON )
option( USE_RTTI                 "Enable RTTI"                                                  ON )
option( COLOR_CMAKE_MESSAGES
                                 "Colorize output from the configuration script.
                                 This comes out all wrong if you use the GUI,
                                 so make sure to set this to false if you do."
                                                                                                TRUE )



###########################################################################################
#                                                                                         #
#                              Prind functions and definitions                            #
#                                                                                         #
###########################################################################################
# https://stackoverflow.com/questions/18968979/how-to-get-colorized-output-with-cmake
# https://gitlab.kitware.com/cmake/cmake/-/issues/16183

if( WIN32 )
   set( COLOR_MSG_SUPPORTED FALSE )
else( )
   set( COLOR_MSG_SUPPORTED TRUE )
endif( )

if( COLOR_CMAKE_MESSAGES AND COLOR_MSG_SUPPORTED AND CMAKE_COLOR_MAKEFILE )
   set( COLOR_CMAKE_MESSAGES TRUE )
else( )
   set( COLOR_CMAKE_MESSAGES FALSE )
endif( )

# Color definitions
string( ASCII 27 _escape )
set( WHITE "29" )
set( GRAY "30" )
set( RED "31" )
set( GREEN "32" )
set( YELLOW "33" )
set( BLUE "34" )
set( MAG "35" )
set( CYAN "36" )
foreach( color WHITE GRAY RED GREEN YELLOW BLUE MAG CYAN )
   set( HI${color} "1\;${${color}}" )
   set( LO${color} "2\;${${color}}" )
   set( _${color}_ "4\;${${color}}" )
   set( _HI${color}_ "1\;4\;${${color}}" )
   set( _LO${color}_ "2\;4\;${${color}}" )
endforeach( )

# Function for printing colored message
# Examples:
#     color_message( GREEN "...." "..." )
#     color_message( "...." "..." )
function( color_message COLOR )
   if( COLOR_CMAKE_MESSAGES )
      set( str "${str}${_escape}[${${COLOR}}m" )
   endif( )

   math( EXPR lastIndex "${ARGC}-1" )
   foreach( index RANGE 1 ${lastIndex} )
      set( arg ${ARGV${index}} )
      set( str "${str}${arg}" )
      # set( str "${str} " )
   endforeach( )

   if( COLOR_CMAKE_MESSAGES )
      set( str "${str}${_escape}[0m" )
   endif( )

   message( STATUS ${str} )
endfunction( )

function( msg_vrb )
   color_message( WHITE ${ARGV} )
endfunction( )


function( msg_dbg )
   color_message( GREEN ${ARGV} )
endfunction( )


function( msg_inf )
   color_message( YELLOW ${ARGV} )
endfunction( )


function( msg_wrn )
   color_message( BLUE ${ARGV} )
endfunction( )


function( msg_err )
   color_message( RED ${ARGV} )
endfunction( )


function( msg_ftl )
   color_message( HIRED ${ARGV} )
endfunction( )



function( message_test )
   message( NOTICE "###########################################################################################" )
   message( NOTICE "#                                                                                         #" )
   message( NOTICE "#                                  MESSAGE TEST (BEGIN):                                  #" )
   message( NOTICE "#                                                                                         #" )
   message( NOTICE "###########################################################################################" )
   msg_vrb( "--------------------------- msg_vrb --------------------------- " )
   msg_dbg( "--------------------------- msg_dbg --------------------------- " )
   msg_inf( "--------------------------- msg_inf --------------------------- " )
   msg_wrn( "--------------------------- msg_wrn --------------------------- " )
   msg_err( "--------------------------- msg_err --------------------------- " )
   msg_ftl( "--------------------------- msg_ftl --------------------------- " )
   message( NOTICE "###########################################################################################" )
   message( NOTICE "#                                                                                         #" )
   message( NOTICE "#                                   MESSAGE TEST (END):                                   #" )
   message( NOTICE "#                                                                                         #" )
   message( NOTICE "###########################################################################################" )
endfunction( )

function( print_environment_variables )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                       '${PROJECT_NAME}' ENVIRONMENT VARIABLES (BEGIN):                    " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
   execute_process( COMMAND "${CMAKE_COMMAND}" "-E" "environment" )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                        '${PROJECT_NAME}' ENVIRONMENT VARIABLES (END):                     " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
endfunction( )

function( print_cmake_variables )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                           '${PROJECT_NAME}' CMAKE VARIABLES (BEGIN):                      " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
   get_cmake_property( _variableNames VARIABLES )
   list( SORT _variableNames )
   foreach (_variableName ${_variableNames})
      msg_vrb( "${_variableName}=${${_variableName}}" )
   endforeach( )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                            '${PROJECT_NAME}' CMAKE VARIABLES (END):                       " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
endfunction( )

function( print_project_files )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                            '${PROJECT_NAME}' project files (BEGIN):                      " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
   msg_vrb( "TARGET_NAME = " ${TARGET_NAME} )

   msg_vrb( "CMAKE_HOME_DIRECTORY = " ${CMAKE_HOME_DIRECTORY} )
   msg_vrb( "CMAKE_CURRENT_LIST_DIR = " ${CMAKE_CURRENT_LIST_DIR} )

   msg_vrb( "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR} )
   msg_vrb( "PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR} )
   msg_vrb( "PROJECT_GEN_DIR = " ${PROJECT_GEN_DIR} )
   msg_vrb( "PROJECT_SOURCE_FILES = " ${PROJECT_SOURCE_FILES} )
   msg_vrb( "PROJECT_GEN_FILES = " ${PROJECT_GEN_FILES} )

   msg_vrb( "CARPC_SOURCE_DIR = " ${CARPC_SOURCE_DIR} )
   msg_vrb( "CARPC_BINARY_DIR = " ${CARPC_BINARY_DIR} )

   msg_vrb( "ROOT_SOURCE_DIR = " ${ROOT_SOURCE_DIR} )
   msg_vrb( "ROOT_BUILD_DIR = " ${ROOT_BUILD_DIR} )
   msg_vrb( "ROOT_DELIVERY_DIR = " ${ROOT_DELIVERY_DIR} )
   msg_vrb( "ROOT_DOCUMENTATION_DIR = " ${ROOT_DOCUMENTATION_DIR} )
   msg_vrb( "ROOT_GEN_DIR = " ${ROOT_GEN_DIR} )

   msg_vrb( "CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX} )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                             '${PROJECT_NAME}' project files (END):                       " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
endfunction( )

function( print_toolchain_config )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                            '${PROJECT_NAME}' toolchain config (BEGIN):                    " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
   msg_vrb( "TOOLCHAIN = $ENV{TOOLCHAIN}" )
   msg_vrb( "SYSROOT = $ENV{SYSROOT}" )
   msg_vrb( "ARCH = $ENV{ARCH}" )
   msg_vrb( "OS = $ENV{OS}" )
   msg_vrb( "API = $ENV{API}" )
   msg_vrb( "TARGET = $ENV{TARGET}" )
   msg_vrb( "SESSION = $ENV{SESSION}" )
   msg_vrb( "AR = $ENV{AR}" )
   msg_vrb( "AS = $ENV{AS}" )
   msg_vrb( "CC = $ENV{CC}" )
   msg_vrb( "CXX = $ENV{CXX}" )
   msg_vrb( "LD = $ENV{LD}" )
   msg_vrb( "RANLIB = $ENV{RANLIB}" )
   msg_vrb( "STRIP = $ENV{STRIP}" )
   msg_dbg( "###########################################################################################" )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "                             '${PROJECT_NAME}' toolchain config (END):                     " )
   msg_dbg( "#                                                                                         #" )
   msg_dbg( "###########################################################################################" )
endfunction( )


###########################################################################################
#                                                                                         #
#                                     Setup functions                                     #
#                                                                                         #
###########################################################################################
macro( setup_target_name )
   get_filename_component( TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
   string( REPLACE " " "_" TARGET_NAME ${TARGET_NAME} )
endmacro( )

macro( setup_gen_dir )
   string( REGEX REPLACE "${ROOT_SOURCE_DIR}" "${ROOT_GEN_DIR}" PROJECT_GEN_DIR ${PROJECT_SOURCE_DIR} )
   file( MAKE_DIRECTORY ${PROJECT_GEN_DIR} )
endmacro( )


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
   msg_vrb( "compiler: Intel C++" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
   msg_vrb( "compiler: Visual Studio C++" )
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
#                                        Thirdparty                                       #
#                                                                                         #
###########################################################################################
set( ANTLR4_JAR ${ROOT_SOURCE_DIR}/thirdparty/antlr/antlr-4.10.1-complete.jar )
set( PLANTUML_JAR ${ROOT_SOURCE_DIR}/thirdparty/plantuml/plantuml-1.2021.14.jar )



###########################################################################################
#                                                                                         #
#                                        Generator                                        #
#                                                                                         #
###########################################################################################
set( SERVICE_GENERATOR ${ROOT_SOURCE_DIR}/build/generator/generator.py )
set( IDL_LEXER ${ROOT_SOURCE_DIR}/build/generator/grammar/IdlLexer.g4 )
set( IDL_PARSER ${ROOT_SOURCE_DIR}/build/generator/grammar/IdlParser.g4 )

# TEMPLATE_ANTLR4_GEN_FILES - List of Python file names produced by antlr4 generator
# for idl file base on lexer and parser grammars
set( TEMPLATE_ANTLR4_GEN_FILES
      IdlLexer.interp IdlLexer.tokens IdlLexer.py
      IdlParser.interp IdlParser.tokens IdlParser.py
      IdlParserListener.py IdlParserVisitor.py
   )
set( ANTLR4_GEN_DIR ${ROOT_GEN_DIR}/grammar/ )
list( TRANSFORM TEMPLATE_ANTLR4_GEN_FILES PREPEND "${ANTLR4_GEN_DIR}" )

# TEMPLATE_IDL_GEN_FILES - List of C++ file names produced by generator for idl file
set( TEMPLATE_IDL_GEN_FILES
      Version.hpp
      Data.hpp Data.cpp
      IServer.hpp IServer.cpp
      ServerImpl.hpp ServerImpl.cpp
      Server.hpp Server.cpp
      IClient.cpp IClient.hpp
      ClientImpl.hpp ClientImpl.cpp
      Client.hpp Client.cpp
   )

# Function for generating C++ source code for server/client base stuff from idl file
# Parameters:
#     IDL_FILE - (in) full path to idl file
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
# Example:
#     generate_idl( ${IDL_FILE} GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_idl IDL_FILE GENERATED_FILES )
   add_custom_command(
         OUTPUT ${TEMPLATE_ANTLR4_GEN_FILES}
         COMMENT "ANTLR4 GENERATION: ${TEMPLATE_ANTLR4_GEN_FILES}"
         COMMAND java -jar ${ANTLR4_JAR} -Dlanguage=Python3 -listener -visitor -o ${ANTLR4_GEN_DIR} ${IDL_LEXER} ${IDL_PARSER}
         DEPENDS ${IDL_LEXER} ${IDL_PARSER}
         VERBATIM
      )

   string( REPLACE ".idl" "/" IDL_GEN_DIR ${IDL_FILE} )
   string( REPLACE "${ROOT_SOURCE_DIR}" "${ROOT_GEN_DIR}" IDL_GEN_DIR ${IDL_GEN_DIR} )
   set( IDL_GEN_FILES ${TEMPLATE_IDL_GEN_FILES} )
   list( TRANSFORM IDL_GEN_FILES PREPEND "${IDL_GEN_DIR}" )
   list( APPEND PROJECT_GEN_FILES ${IDL_GEN_FILES} )
   set( ${GENERATED_FILES} ${IDL_GEN_FILES} PARENT_SCOPE )

   msg_inf( "Generating C++ files from '" ${IDL_FILE} "':" )
   foreach( FILE ${IDL_GEN_FILES} )
      msg_vrb( "   " ${FILE} )
   endforeach( )

   add_custom_command(
         OUTPUT ${IDL_GEN_FILES}
         COMMENT "CODE GENERATION: ${IDL_GEN_FILES}"
         COMMAND ${SERVICE_GENERATOR}
               # --antlr_jar=${ANTLR4_JAR}
               # --antlr_lexer=${IDL_LEXER}
               # --antlr_parser=${IDL_PARSER}
               --antlr_outdir=${ANTLR4_GEN_DIR}
               --source=${IDL_FILE}
               --gen_outdir=${IDL_GEN_DIR}
         DEPENDS ${IDL_FILE} ${TEMPLATE_ANTLR4_GEN_FILES}
         VERBATIM
      )
endfunction( )

# Function for generating C++ source code for server/client base stuff from several idl files
# Parameters:
#     IDL_FILES - (in) list of full pathes to idl files
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
#                       from all idls
# Example:
#     generate_idls( "${IDL_FILES}" GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_idls IDL_FILES GENERATED_FILES )
   set( IDLS_GENERATED_FILES "" )
   foreach( IDL_FILE ${IDL_FILES} )
      generate_idl( ${IDL_FILE} IDL_GENERATED_FILES )
      list( APPEND IDLS_GENERATED_FILES ${IDL_GENERATED_FILES} )
   endforeach( )
   set( ${GENERATED_FILES} ${IDLS_GENERATED_FILES} PARENT_SCOPE )
endfunction( )




###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_project_files( )
print_toolchain_config( )



add_subdirectory( tracing )
add_subdirectory( framework )
add_subdirectory( servicebrocker )
add_subdirectory( service )
add_subdirectory( hooks )
add_subdirectory( instrumental )
add_subdirectory( examples )
