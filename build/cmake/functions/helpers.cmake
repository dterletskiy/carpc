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
#     is_on_off( RESULT VARIABLE )
#     msg_err( "RESULT = " ${RESULT} )
# Example:
#     is_on_off( RESULT VARIABLE ON )
#     msg_err( "RESULT = " ${RESULT} )
function( is_on_off OUT_RESULT IN_VALUE )
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
