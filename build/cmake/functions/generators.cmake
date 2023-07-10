###########################################################################################
#                                                                                         #
#                                        Generator                                        #
#                                                                                         #
###########################################################################################
set( CARPC_GENERATOR ${ROOT_SOURCE_DIR}/build/generator/generator.py )
set( BASE_LEXER ${ROOT_SOURCE_DIR}/build/generator/grammar/XdlLexer.g4 )
set( BASE_PARSER ${ROOT_SOURCE_DIR}/build/generator/grammar/XdlParser.g4 )
set( IDL_LEXER ${ROOT_SOURCE_DIR}/build/generator/grammar/IdlLexer.g4 )
set( IDL_PARSER ${ROOT_SOURCE_DIR}/build/generator/grammar/IdlParser.g4 )
set( DDL_LEXER ${ROOT_SOURCE_DIR}/build/generator/grammar/DdlLexer.g4 )
set( DDL_PARSER ${ROOT_SOURCE_DIR}/build/generator/grammar/DdlParser.g4 )
set( ADL_LEXER ${ROOT_SOURCE_DIR}/build/generator/grammar/AdlLexer.g4 )
set( ADL_PARSER ${ROOT_SOURCE_DIR}/build/generator/grammar/AdlParser.g4 )

set( ANTLR4_GEN_DIR ${ROOT_GEN_DIR}/grammar/ )



# TEMPLATE_ANTLR4_IDL_GEN_FILES - List of Python file names produced by antlr4 generator
# for idl file base on lexer and parser grammars
set( TEMPLATE_ANTLR4_IDL_GEN_FILES
      IdlLexer.interp IdlLexer.tokens IdlLexer.py
      IdlParser.interp IdlParser.tokens IdlParser.py
      IdlParserListener.py IdlParserVisitor.py
   )
list( TRANSFORM TEMPLATE_ANTLR4_IDL_GEN_FILES PREPEND "${ANTLR4_GEN_DIR}" )

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



# TEMPLATE_ANTLR4_DDL_GEN_FILES - List of Python file names produced by antlr4 generator
# for ddl file base on lexer and parser grammars
set( TEMPLATE_ANTLR4_DDL_GEN_FILES
      DdlLexer.interp DdlLexer.tokens DdlLexer.py
      DdlParser.interp DdlParser.tokens DdlParser.py
      DdlParserListener.py DdlParserVisitor.py
   )
list( TRANSFORM TEMPLATE_ANTLR4_DDL_GEN_FILES PREPEND "${ANTLR4_GEN_DIR}" )

# TEMPLATE_DDL_GEN_FILES - List of C++ file names produced by generator for ddl file
set( TEMPLATE_DDL_GEN_FILES
      Types.hpp Types.cpp
   )



# TEMPLATE_ANTLR4_ADL_GEN_FILES - List of Python file names produced by antlr4 generator
# for adl file base on lexer and parser grammars
set( TEMPLATE_ANTLR4_ADL_GEN_FILES
      AdlLexer.interp AdlLexer.tokens AdlLexer.py
      AdlParser.interp AdlParser.tokens AdlParser.py
      AdlParserListener.py AdlParserVisitor.py
   )
list( TRANSFORM TEMPLATE_ANTLR4_ADL_GEN_FILES PREPEND "${ANTLR4_GEN_DIR}" )

# TEMPLATE_ADL_GEN_FILES - List of C++ file names produced by generator for adl file
set( TEMPLATE_ADL_GEN_FILES
      main.cpp
   )



# Function for generating C++ source code for server/client base stuff from xdl file
# Parameters:
#     XDL_FILE - (in) full path to xdl file
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
# Example:
#     generate_xdl( ${XDL_FILE} GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_xdl XDL_FILE GENERATED_FILES )
   get_filename_component( _FILE_NAME_ ${XDL_FILE} NAME )
   string( REGEX MATCH "^(.*)(\\.[^.]*)$" dummy ${_FILE_NAME_} )
   # *.xdl file name witout path and file extension
   set( XDL_FILE_NAME         ${CMAKE_MATCH_1} )
   # *.xdl file extension
   set( XDL_FILE_EXTENTION    ${CMAKE_MATCH_2} )
   # *.xdl location directory
   get_filename_component( XDL_FILE_DIR ${XDL_FILE} DIRECTORY )
   # *.xdl generated destination directory
   string( REPLACE "${ROOT_SOURCE_DIR}" "${ROOT_GEN_DIR}" XDL_GEN_DIR "${XDL_FILE_DIR}/${XDL_FILE_NAME}/" )

   msg_inf( "processing xdl type: " ${XDL_FILE_EXTENTION} )

   if( ".adl" STREQUAL ${XDL_FILE_EXTENTION} )
      set( TEMPLATE_ANTLR4_GEN_FILES      ${TEMPLATE_ANTLR4_ADL_GEN_FILES} )
      set( TEMPLATE_CPP_GEN_FILES         ${TEMPLATE_ADL_GEN_FILES}        )
      set( XDL_LEXER                      ${ADL_LEXER}                     )
      set( XDL_PARSER                     ${ADL_PARSER}                    )
   elseif( ".idl" STREQUAL ${XDL_FILE_EXTENTION} )
      set( TEMPLATE_ANTLR4_GEN_FILES      ${TEMPLATE_ANTLR4_IDL_GEN_FILES} )
      set( TEMPLATE_CPP_GEN_FILES         ${TEMPLATE_IDL_GEN_FILES}        )
      set( XDL_LEXER                      ${IDL_LEXER}                     )
      set( XDL_PARSER                     ${IDL_PARSER}                    )
   elseif( ".ddl" STREQUAL ${XDL_FILE_EXTENTION} )
      set( TEMPLATE_ANTLR4_GEN_FILES      ${TEMPLATE_ANTLR4_DDL_GEN_FILES} )
      set( TEMPLATE_CPP_GEN_FILES         ${TEMPLATE_DDL_GEN_FILES}        )
      set( XDL_LEXER                      ${DDL_LEXER}                     )
      set( XDL_PARSER                     ${DDL_PARSER}                    )
   else( )
      msg_err( "undefined xdl type" )
   endif( )

   add_custom_command(
         OUTPUT ${TEMPLATE_ANTLR4_GEN_FILES}
         COMMENT "ANTLR4 GENERATION: ${TEMPLATE_ANTLR4_GEN_FILES}"
         COMMAND java -jar ${ANTLR4_JAR} -Dlanguage=Python3 -listener -visitor -o ${ANTLR4_GEN_DIR} ${XDL_LEXER} ${XDL_PARSER}
         DEPENDS ${XDL_LEXER} ${XDL_PARSER}
         VERBATIM
      )

   set( XDL_GEN_FILES ${TEMPLATE_CPP_GEN_FILES} )
   list( TRANSFORM XDL_GEN_FILES PREPEND "${XDL_GEN_DIR}" )
   set( ${GENERATED_FILES} ${XDL_GEN_FILES} PARENT_SCOPE )

   msg_inf( "Generating C++ files from '" ${XDL_FILE} "':" )
   foreach( FILE ${XDL_GEN_FILES} )
      msg_vrb( "   " ${FILE} )
   endforeach( )

   add_custom_command(
         OUTPUT ${XDL_GEN_FILES}
         COMMENT "CODE GENERATION: ${XDL_GEN_FILES}"
         DEPENDS ${XDL_FILE} ${TEMPLATE_ANTLR4_GEN_FILES}
         COMMAND ${CARPC_GENERATOR}
               --include=${PFW_DIR}
               --antlr_outdir=${ANTLR4_GEN_DIR}
               --source=${XDL_FILE}
               --gen_outdir=${XDL_GEN_DIR}
         DEPENDS ${XDL_FILE} ${TEMPLATE_ANTLR4_GEN_FILES}
         VERBATIM
      )
endfunction( )

# Function for generating C++ source code for server/client base stuff from several xdl files
# Parameters:
#     XDL_FILES - (in) list of full pathes to xdl files
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
#                       from all xdls
# Example:
#     generate_xdls( "${XDL_FILES}" GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_xdls XDL_FILES GENERATED_FILES )
   set( XDLS_GENERATED_FILES "" )
   foreach( XDL_FILE ${XDL_FILES} )
      generate_xdl( ${XDL_FILE} XDL_GENERATED_FILES )
      list( APPEND XDLS_GENERATED_FILES ${XDL_GENERATED_FILES} )
   endforeach( )
   set( ${GENERATED_FILES} ${XDLS_GENERATED_FILES} PARENT_SCOPE )
endfunction( )



# TEMPLATE_GPB_GEN_FILES - List of C++ file extentions produced by generator for proto file
set( TEMPLATE_GPB_GEN_FILES
      .pb.cc
      .pb.h
   )

# Function for generating C++ source code base from proto file
# Parameters:
#     PROTO_FILE - (in) full path to proto file
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
# Example:
#     generate_gpb( ${PROTO_FILE} GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_gpb PROTO_FILE GENERATED_FILES )
   get_filename_component( _FILE_NAME_ ${PROTO_FILE} NAME )
   string( REGEX MATCH "^(.*)(\\.[^.]*)$" dummy ${_FILE_NAME_} )
   # *.xdl file name witout path and file extension
   set( PROTO_FILE_NAME       ${CMAKE_MATCH_1} )
   # *.xdl file extension
   set( PROTO_FILE_EXTENTION  ${CMAKE_MATCH_2} )
   # *.xdl location directory
   get_filename_component( PROTO_FILE_DIR ${PROTO_FILE} DIRECTORY )
   # *.xdl generated destination directory
   string( REPLACE "${ROOT_SOURCE_DIR}" "${ROOT_GEN_DIR}" PROTO_GEN_DIR "${PROTO_FILE_DIR}/" )

   file( MAKE_DIRECTORY ${PROTO_GEN_DIR} )

   set( PROTO_GEN_FILES ${TEMPLATE_GPB_GEN_FILES} )
   list( TRANSFORM PROTO_GEN_FILES PREPEND "${PROTO_FILE_NAME}" )
   list( TRANSFORM PROTO_GEN_FILES PREPEND "${PROTO_GEN_DIR}/" )
   set( ${GENERATED_FILES} ${PROTO_GEN_FILES} PARENT_SCOPE )

   add_custom_command(
         OUTPUT ${PROTO_GEN_FILES}
         COMMENT "GPB GENERATION: ${PROTO_GEN_FILES}"
         COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} -I${PROTO_FILE_DIR} --cpp_out=${PROTO_GEN_DIR} ${PROTO_FILE}
         DEPENDS ${PROTO_FILE}
         VERBATIM
      )
endfunction( )

# Function for generating C++ source code base from several proto files
# Parameters:
#     PROTO_FILES - (in) list of full pathes to proto files
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
# Example:
#     generate_gpbs( ${PROTO_FILES} GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_gpbs PROTO_FILES GENERATED_FILES )
   set( PROTOS_GENERATED_FILES "" )
   foreach( PROTO_FILE ${PROTO_FILES} )
      generate_gpb( ${PROTO_FILE} PROTO_GENERATED_FILES )
      list( APPEND PROTOS_GENERATED_FILES ${PROTO_GENERATED_FILES} )
   endforeach( )
   set( ${GENERATED_FILES} ${PROTOS_GENERATED_FILES} PARENT_SCOPE )
endfunction( )




# TEMPLATE_PLANTUML_GEN_FILES - List of file extentions produced by generator for plantuml file
set( TEMPLATE_PLANTUML_GEN_FILES
      .png
   )

# Function for generating files from plantuml file
# Parameters:
#     PLANTUML_FILE - (in) full path to plantuml file
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
# Example:
#     generate_gpb( ${PLANTUML_FILE} GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_plantuml PLANTUML_FILE GENERATED_FILES )
   get_filename_component( _FILE_NAME_ ${PLANTUML_FILE} NAME )
   string( REGEX MATCH "^(.*)(\\.[^.]*)$" dummy ${_FILE_NAME_} )
   # *.plantuml file name witout path and file extension
   set( PLANTUML_FILE_NAME       ${CMAKE_MATCH_1} )
   # *.plantuml file extension
   set( PLANTUML_FILE_EXTENTION  ${CMAKE_MATCH_2} )
   # *.plantuml location directory
   get_filename_component( PLANTUML_FILE_DIR ${PLANTUML_FILE} DIRECTORY )

   string( REPLACE "${ROOT_SOURCE_DIR}" "${ROOT_GEN_DIR}" PLANTUML_GEN_DIR ${PLANTUML_FILE_DIR} )
   set( PLANTUML_GEN_FILES ${TEMPLATE_PLANTUML_GEN_FILES} )
   list( TRANSFORM PLANTUML_GEN_FILES PREPEND "${PLANTUML_FILE_NAME}" )
   list( TRANSFORM PLANTUML_GEN_FILES PREPEND "${PLANTUML_GEN_DIR}/" )
   set( ${GENERATED_FILES} ${PLANTUML_GEN_FILES} PARENT_SCOPE )

   add_custom_command(
         OUTPUT ${PLANTUML_GEN_FILES}
         COMMENT "PLANTUML GENERATION: ${PLANTUML_GEN_FILES}"
         COMMAND java -jar ${PLANTUML_JAR} -tpng -progress -V -debugsvek -o ${PLANTUML_GEN_DIR} ${PLANTUML_FILE}
         DEPENDS ${PLANTUML_FILE}
         VERBATIM
      )
endfunction( )

# Function for generating files from several plantuml files
# Parameters:
#     PLANTUML_FILES - (in) list of full pathes to plantuml files
#     GENERATED_FILES - (out) variable name that will be fullfilled by list of generated files
# Example:
#     generate_gpbs( ${PLANTUML_FILES} GEN_FILES )
#     msg_dbg( "GEN_FILES = " ${GEN_FILES} )
function( generate_plantumls PLANTUML_FILES GENERATED_FILES )
   set( PLANTUMLS_GENERATED_FILES "" )
   foreach( PLANTUML_FILE ${PLANTUML_FILES} )
      generate_plantuml( ${PLANTUML_FILE} PLANTUML_GENERATED_FILES )
      list( APPEND PLANTUMLS_GENERATED_FILES ${PLANTUML_GENERATED_FILES} )
   endforeach( )
   set( ${GENERATED_FILES} ${PLANTUMLS_GENERATED_FILES} PARENT_SCOPE )
endfunction( )

# Generate graph file with dependancies between targets.
# Parameters:
#     IN_DESTINATION - (in) destination directory
#     IN_NAME - (in) destination graph name
#     IN_TARGETS - (in) list of targets
# Example:
# get_all_targets( TARGETS )
# generate_dependencies_graph( TARGETS )
function( generate_dependencies_graph IN_DESTINATION IN_NAME IN_TARGETS )
   set( DOT_FILE "${IN_DESTINATION}/${IN_NAME}.dot" )
   set( PNG_FILE "${IN_DESTINATION}/${IN_NAME}.png" )

   file( WRITE ${DOT_FILE} "digraph ${IN_NAME} {\n" )
   foreach( target IN LISTS IN_TARGETS )
      file( APPEND ${DOT_FILE} "  \"${target}\"\n" )

      get_property( target_deps TARGET ${target} PROPERTY INTERFACE_LINK_LIBRARIES )
      foreach( dep IN LISTS target_deps )
         # file( APPEND ${DOT_FILE} "  \"${dep}\" -> \"${target}\"\n" )
         file( APPEND ${DOT_FILE} "  \"${target}\" -> \"${dep}\"\n" )
      endforeach( )
   endforeach( )
   file( APPEND ${DOT_FILE} "}\n" )

   execute_process(
      COMMAND bash -c "dot -Tpng ${DOT_FILE} -o ${PNG_FILE}"
      OUTPUT_VARIABLE output_variable
      RESULT_VARIABLE result_variable
      OUTPUT_STRIP_TRAILING_WHITESPACE
   )

   msg_dbg( "Output: ${output_variable}" )
   msg_dbg( "Result: ${result_variable}" )


   # add_custom_command(
   #    OUTPUT ${PNG_FILE}
   #    COMMENT "PNG graph file generation"
   #    COMMAND dot -Tpng ${DOT_FILE} -o ${PNG_FILE}
   #    DEPENDS ${DOT_FILE}
   #    VERBATIM
   # )
endfunction( )
