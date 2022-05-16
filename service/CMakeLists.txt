cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( SERVICE )



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
set( ANTLR4_JAR ${ROOT_SOURCE_DIR}/thirdparty/antlr/antlr-4.10.1-complete.jar )
set( LEXER ${PROJECT_SOURCE_DIR}/grammar/IdlLexer.g4 )
set( PARSER ${PROJECT_SOURCE_DIR}/grammar/IdlParser.g4 )
set( GENERATOR ${ROOT_SOURCE_DIR}/build/generator/generator.py )

set( TEMPLATE_ANTLR4_GEN_FILES
      IdlLexer.interp IdlLexer.tokens IdlLexer.py
      IdlParser.interp IdlParser.tokens IdlParser.py
      IdlParserListener.py IdlParserVisitor.py
   )
set( ANTLR4_GEN_DIR ${PROJECT_GEN_DIR}/grammar/ )
list( TRANSFORM TEMPLATE_ANTLR4_GEN_FILES PREPEND "${ANTLR4_GEN_DIR}" )

add_custom_command(
      OUTPUT ${TEMPLATE_ANTLR4_GEN_FILES}
      COMMENT "ANTLR4 GENERATION: ${TEMPLATE_ANTLR4_GEN_FILES}"
      COMMAND java -jar ${ANTLR4_JAR} -Dlanguage=Python3 -listener -visitor -o ${ANTLR4_GEN_DIR} ${LEXER} ${PARSER}
      DEPENDS ${LEXER} ${PARSER}
      VERBATIM
   )



file( GLOB_RECURSE IDL_FILES ${PROJECT_SOURCE_DIR}/*.idl )

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

set( GEN_FILES "" )
foreach( IDL_FILE ${IDL_FILES} )
   string( REPLACE ".idl" "/" IDL_GEN_DIR ${IDL_FILE} )
   string( REPLACE "${PROJECT_SOURCE_DIR}" "${PROJECT_GEN_DIR}" IDL_GEN_DIR ${IDL_GEN_DIR} )
   set( IDL_GEN_FILES ${TEMPLATE_IDL_GEN_FILES} )
   list( TRANSFORM IDL_GEN_FILES PREPEND "${IDL_GEN_DIR}" )
   list( APPEND GEN_FILES ${IDL_GEN_FILES} )

   add_custom_command(
         OUTPUT ${IDL_GEN_FILES}
         COMMENT "CODE GENERATION: ${IDL_GEN_FILES}"
         COMMAND ${GENERATOR}
               # --antlr_jar=${ANTLR4_JAR}
               # --antlr_lexer=${LEXER}
               # --antlr_parser=${PARSER}
               --antlr_outdir=${ANTLR4_GEN_DIR}
               --source=${IDL_FILE}
               --gen_outdir=${IDL_GEN_DIR}
         DEPENDS ${IDL_FILE} ${TEMPLATE_ANTLR4_GEN_FILES}
         VERBATIM
      )
endforeach( )



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



add_library( ${TARGET_NAME} SHARED ${PROJECT_SOURCE_FILES} ${GEN_FILES} )

target_link_libraries( ${TARGET_NAME} tracing )
target_link_libraries( ${TARGET_NAME} framework )

install( TARGETS ${TARGET_NAME} DESTINATION lib )
install( DIRECTORY ${PROJECT_SOURCE_DIR}/api DESTINATION include )
