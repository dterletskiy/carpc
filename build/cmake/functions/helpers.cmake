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
