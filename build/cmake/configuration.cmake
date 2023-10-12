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
carpc_is_on_off( SYS_TRACE SYS_TRACE ON )

# Enable application tracing
carpc_is_on_off( MSG_TRACE MSG_TRACE ON )

# Enable colored tracing for console
carpc_is_on_off( COLORED_TRACE COLORED_TRACE ON )

# Enable dlt tracing
carpc_is_on_off( DLT_TRACE DLT_TRACE ON )

# Enable dlt tracing
carpc_is_on_off( DEBUG_STREAM DEBUG_STREAM OFF )

# Enable memory allocator hooks
carpc_is_on_off( MEMORY_HOOK MEMORY_HOOK OFF )

# Enable instrumental functionality
carpc_is_on_off( INSTRUMENTAL INSTRUMENTAL OFF )

# Enable debug information
carpc_is_on_off( USE_DEBUG USE_DEBUG OFF )

# Enable goolgle protobuf
carpc_is_on_off( USE_GPB USE_GPB ON )

# Enable RTTI
carpc_is_on_off( USE_RTTI USE_RTTI ON )




set( ANTLR4_JAR ${ROOT_SOURCE_DIR}/thirdparty/antlr/antlr-4.10.1-complete.jar )
set( PLANTUML_JAR ${ROOT_SOURCE_DIR}/thirdparty/plantuml/plantuml-1.2021.14.jar )
