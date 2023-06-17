# set(CMAKE_GENERATOR "Sublime Text 2 - Unix Makefiles")

set( CMAKE_VERBOSE_MAKEFILE on )

# Root source directory
set( ROOT_SOURCE_DIR ${CMAKE_SOURCE_DIR} )

# Root binary directory
set( ROOT_BINARY_DIR ${CMAKE_BINARY_DIR} )

# Root gen directory
if( NOT DEFINED ROOT_GEN_DIR )
   set( ROOT_GEN_DIR ${CMAKE_BINARY_DIR}/gen )
endif( )

# Root delivery directory
if( NOT DEFINED ROOT_DELIVERY_DIR )
   set( ROOT_DELIVERY_DIR ${CMAKE_INSTALL_PREFIX} )
else( )
   set( CMAKE_INSTALL_PREFIX ${ROOT_DELIVERY_DIR} )
endif( )

# Python FW directory
if( NOT DEFINED PFW_DIR )
   set( PFW_DIR ${CMAKE_SOURCE_DIR}/submodules/dterletskiy/python_fw )
endif( )

# Target OS
if( NOT DEFINED TARGET_OS )
   set( TARGET_OS "linux" )
endif( )




# Enable framework system tracin
is_on_off( SYS_TRACE SYS_TRACE ON )

# Enable application tracing
is_on_off( MSG_TRACE MSG_TRACE ON )

# Enable colored tracing for console
is_on_off( COLORED_TRACE COLORED_TRACE ON )

# Enable dlt tracing
is_on_off( DLT_TRACE DLT_TRACE ON )

# Enable memory allocator hooks
is_on_off( MEMORY_HOOK MEMORY_HOOK OFF )

# Enable instrumental functionality
is_on_off( INSTRUMENTAL INSTRUMENTAL OFF )

# Enable debug information
is_on_off( USE_DEBUG USE_DEBUG OFF )

# Enable goolgle protobuf
is_on_off( USE_GPB USE_GPB ON )

# Enable RTTI
is_on_off( USE_RTTI USE_RTTI ON )




set( ANTLR4_JAR ${ROOT_SOURCE_DIR}/thirdparty/antlr/antlr-4.10.1-complete.jar )
set( PLANTUML_JAR ${ROOT_SOURCE_DIR}/thirdparty/plantuml/plantuml-1.2021.14.jar )



function( print_configuration )
   msg_dbg( "SYS_TRACE        = " ${SYS_TRACE} )
   msg_dbg( "MSG_TRACE        = " ${MSG_TRACE} )
   msg_dbg( "COLORED_TRACE    = " ${COLORED_TRACE} )
   msg_dbg( "DLT_TRACE        = " ${DLT_TRACE} )
   msg_dbg( "MEMORY_HOOK      = " ${MEMORY_HOOK} )
   msg_dbg( "INSTRUMENTAL     = " ${INSTRUMENTAL} )
   msg_dbg( "USE_DEBUG        = " ${USE_DEBUG} )
   msg_dbg( "USE_GPB          = " ${USE_GPB} )
   msg_dbg( "USE_RTTI         = " ${USE_RTTI} )
endfunction( )
