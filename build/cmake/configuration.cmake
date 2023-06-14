option( SYS_TRACE                "Enable framework system tracing"                           ON )
option( MSG_TRACE                "Enable application tracing"                                ON )
option( COLORED_TRACE            "Enable colored tracing for console"                        ON )
option( DLT_TRACE                "Enable dlt tracing"                                        ON )
option( MEMORY_HOOK              "Enable memory allocator hooks"                             OFF )
option( INSTRUMENTAL             "Enable instrumental functionality"                         OFF )
option( USE_DEBUG                "Enable debug information"                                  OFF )
option( USE_GPB                  "Enable goolgle protobuf"                                   ON )
option( USE_RTTI                 "Enable RTTI"                                               ON )



# set(CMAKE_GENERATOR "Sublime Text 2 - Unix Makefiles")

set( CMAKE_VERBOSE_MAKEFILE on )

set( ROOT_SOURCE_DIR ${CMAKE_SOURCE_DIR} )

if( NOT DEFINED ROOT_GEN_DIR )
   set( ROOT_GEN_DIR ${CMAKE_BINARY_DIR}/gen )
endif( )

if( NOT DEFINED ROOT_DELIVERY_DIR )
   set( ROOT_DELIVERY_DIR ${CMAKE_INSTALL_PREFIX} )
else( )
   set( CMAKE_INSTALL_PREFIX ${ROOT_DELIVERY_DIR} )
endif( )


if( NOT DEFINED PFW_DIR )
   set( PFW_DIR ${CMAKE_SOURCE_DIR}/submodules/dterletskiy/python_fw )
endif( )

if( NOT DEFINED TARGET_OS )
   set( TARGET_OS "linux" )
endif( )



set( ANTLR4_JAR ${ROOT_SOURCE_DIR}/thirdparty/antlr/antlr-4.10.1-complete.jar )
set( PLANTUML_JAR ${ROOT_SOURCE_DIR}/thirdparty/plantuml/plantuml-1.2021.14.jar )



set( EXTENTIONS_CPP_SRC c cpp cxx )
set( EXTENTIONS_CPP_HDR h hpp hxx )
set( EXTENTIONS_CPP h hpp hxx c cpp cxx )
set( EXTENTIONS_PLANTUML plantuml )
set( EXTENTIONS_ADL adl )
set( EXTENTIONS_IDL idl )
set( EXTENTIONS_DDL ddl )
set( EXTENTIONS_PROTO proto )

