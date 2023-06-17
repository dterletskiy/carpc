###########################################################################################
#                                                                                         #
#                                     Setup functions                                     #
#                                                                                         #
###########################################################################################

# Generate path to generated files based on path to source files.
function( setup_gen_dir )
   string( REGEX REPLACE "${ROOT_SOURCE_DIR}" "${ROOT_GEN_DIR}" PROJECT_GEN_DIR ${PROJECT_SOURCE_DIR} )
   file( MAKE_DIRECTORY ${PROJECT_GEN_DIR} )
endfunction( )

# Calculate target name as a last directory from the location path and put it to TARGET_NAME variable.
# Later on this variable is used as a name for building target using add_library(...) and other functions.
# Generate source files list and put them to PROJECT_SOURCE_FILES variable.
# Parameters:
#     OUT_TARGET_NAME - (out) calculated target name
# Example:
#     setup_target_name( TARGET_NAME )
#     msg_dbg( "TARGET_NAME = " ${TARGET_NAME} )
function( setup_target_name OUT_TARGET_NAME )
   get_filename_component( LOCAL_TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
   string( REPLACE " " "_" LOCAL_TARGET_NAME ${LOCAL_TARGET_NAME} )
   set( ${OUT_TARGET_NAME} ${LOCAL_TARGET_NAME} PARENT_SCOPE )
endfunction( )

# Calculate target name and generate path to generated files.
function( setup_project OUT_TARGET_NAME )
   setup_target_name( LOCAL_TARGET_NAME )
   set( ${OUT_TARGET_NAME} ${LOCAL_TARGET_NAME} PARENT_SCOPE )
   setup_gen_dir( )
endfunction( )

# Find all files with defined extentions in defined directory and put them to OUT_FILES variable.
# Parameters:
#     IN_LOCATION - (in) directory where recursive search will be performed
#     IN_EXTENTIONS - (in) file extentions what should be found
#     OUT_FILES - (out) list of detected files
# Example:
#     find_files_by_ext( PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR} "${CPP_EXTENTIONS}" )
#     msg_dbg( "PROJECT_SOURCE_FILES = " ${PROJECT_SOURCE_FILES} )
function( find_files_by_ext OUT_FILES IN_LOCATION IN_EXTENTIONS )
   set( LOCAL_FILES "" )
   foreach( EXTENTION ${IN_EXTENTIONS} )
      file( GLOB_RECURSE _FILES_ ${IN_LOCATION}/*.${EXTENTION} )
      list( APPEND LOCAL_FILES ${_FILES_} )
   endforeach( )
   set( ${OUT_FILES} ${LOCAL_FILES} PARENT_SCOPE )
endfunction( )

# Find all files with defined extentions in defined directory, add them to
# provided list of files and put result to OUT_FILES variable.
# Parameters:
#     IN_LOCATION - (in) directory where recursive search will be performed
#     IN_EXTENTIONS - (in) file extentions what should be found
#     IN_FILES - (in) files to be extended
#     OUT_FILES - (out) list of detected files and provided files
# Example:
#     msg_dbg( "PROJECT_SOURCE_FILES = " ${PROJECT_SOURCE_FILES} )
#     add_files_by_ext( PROJECT_SOURCE_FILES "${PROJECT_SOURCE_FILES}" ${PROJECT_SOURCE_DIR} "${CPP_EXTENTIONS}" )
#     msg_dbg( "PROJECT_SOURCE_FILES = " ${PROJECT_SOURCE_FILES} )
function( add_files_by_ext OUT_FILES IN_FILES IN_LOCATION IN_EXTENTIONS )
   set( LOCAL_FILES "" )
   find_files_by_ext( LOCAL_FILES ${IN_LOCATION} "${IN_EXTENTIONS}" )

   list( APPEND IN_FILES ${LOCAL_FILES} )
   set( ${OUT_FILES} ${IN_FILES} PARENT_SCOPE )
endfunction( )
