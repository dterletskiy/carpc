set( POSITIVE_VALUES "true" "TRUE" "yes" "YES" "on" "ON" )
set( NEGATIVE_VALUES "false" "FALSE" "no" "NO" "off" "OFF" )



# Test string the variable for positive or negative boolean value.
# In this way, the call parameters can be checked.
# Parameters:
#     OUT_RESULT - (out) test result ON/OFF
#     IN_VALUE - (in) variable to test
#     IN_DEFAULT_VALUE - (in optional) default result in case if IN_VALUE is not defined
# Example:
#     set( VARIABLE "yes" )
#     carpc_is_on_off( RESULT VARIABLE )
#     msg_err( "RESULT = " ${RESULT} )
# Example:
#     carpc_is_on_off( RESULT VARIABLE ON )
#     msg_err( "RESULT = " ${RESULT} )
function( carpc_is_on_off OUT_RESULT IN_VALUE )
   set( RESULT_POSITIVE ON )
   set( RESULT_NEGATIVE OFF )

   if( ${ARGC} GREATER 2 )
      set( IN_DEFAULT_VALUE ${ARGV2} )
   else( )
      set( IN_DEFAULT_VALUE ${RESULT_NEGATIVE} )
   endif( )

   if( NOT DEFINED ${IN_VALUE} )
      set( ${OUT_RESULT} ${IN_DEFAULT_VALUE} PARENT_SCOPE )
   else( )
      set( ${OUT_RESULT} ${RESULT_NEGATIVE} PARENT_SCOPE )

      foreach( POSITIVE_ITEM IN LISTS POSITIVE_VALUES )
         if( ${POSITIVE_ITEM} STREQUAL ${IN_VALUE} )
            set( ${OUT_RESULT} ${RESULT_POSITIVE} PARENT_SCOPE )
            break( )
         endif( )
      endforeach( )
   endif( )
endfunction( )



# Function for searching all targets defined in all current directory and all
# subdirectories.
# link: https://stackoverflow.com/a/62311397
# Parameters:
#     OUT_TARGETS - (out) list of targets
# Example:
# carpc_get_all_targets( TARGETS )
# msg_dbg( "List of all targets" )
# foreach( TARGET IN LISTS TARGETS )
#    msg_dbg( " - '${TARGET}'" )
# endforeach( )
function( carpc_get_all_targets OUT_TARGETS )
   set( LOCAL_TARGETS )
   __all_targets_recursive__( LOCAL_TARGETS ${CMAKE_CURRENT_SOURCE_DIR} )
   set( ${OUT_TARGETS} ${LOCAL_TARGETS} PARENT_SCOPE )
endfunction( )

# Helper macro for "carpc_get_all_targets" function
macro( __all_targets_recursive__ OUT_TARGETS IN_DIR )
   get_property( SUBDIRS DIRECTORY ${IN_DIR} PROPERTY SUBDIRECTORIES )
   foreach( SUBDIR ${SUBDIRS} )
   __all_targets_recursive__( ${OUT_TARGETS} ${SUBDIR} )
   endforeach( )

   get_property( CURRENT_TARGETS DIRECTORY ${IN_DIR} PROPERTY BUILDSYSTEM_TARGETS )
   list( APPEND ${OUT_TARGETS} ${CURRENT_TARGETS} )
endmacro( )




# Calculate target name as a last directory from the location path
# and put it to PROJECT_TARGET_NAME variable.
# Later on this variable is used as a name for building target using add_library(...)
# and other functions.
# Generate source files list and put them to PROJECT_SOURCE_FILES variable.
# Example:
#     carpc_setup_target_name( )
#     msg_dbg( "PROJECT_TARGET_NAME = " ${PROJECT_TARGET_NAME} )
function( carpc_setup_target_name )
   get_filename_component( TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
   string( REPLACE " " "_" TARGET_NAME ${TARGET_NAME} )

   set( PROJECT_TARGET_NAME  "${TARGET_NAME}" CACHE INTERNAL "PROJECT_TARGET_NAME" )
   set( ${PROJECT_NAME}_TARGET_NAME  "${TARGET_NAME}" CACHE INTERNAL "${PROJECT_NAME}_TARGET_NAME" )
endfunction( )

# Generate path to generated files based on path to source files.
# Define variables PROJECT_GEN_DIR and <PROJECT-NAME>_GEN_DIR
function( carpc_setup_gen_dir )
   string( REGEX REPLACE "${ROOT_SOURCE_DIR}" "${ROOT_GEN_DIR}" GEN_DIR ${PROJECT_SOURCE_DIR} )
   set( PROJECT_GEN_DIR  "${GEN_DIR}" CACHE INTERNAL "PROJECT_GEN_DIR" )
   set( ${PROJECT_NAME}_GEN_DIR  "${GEN_DIR}" CACHE INTERNAL "${PROJECT_NAME}_GEN_DIR" )

   file( MAKE_DIRECTORY ${PROJECT_GEN_DIR} )
endfunction( )

function( carpc_add_subdirectory IN_SUBDIR )
   add_subdirectory( ${IN_SUBDIR} )
   carpc_setup_gen_dir( )
endfunction( )

macro( carpc_project IN_PROJECT_NAME )
   project( ${IN_PROJECT_NAME} )
   carpc_setup_target_name( PROJECT_TARGET_NAME )
   carpc_setup_gen_dir( )
endmacro( )
