option( COLOR_CMAKE_MESSAGES     "Colorize output from the configuration script.
                                 This comes out all wrong if you use the GUI,
                                 so make sure to set this to false if you do."                  TRUE )

###########################################################################################
#                                                                                         #
#                              Print functions and definitions                            #
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

   # math( EXPR lastIndex "${ARGC}-1" )
   # foreach( index RANGE 1 ${lastIndex} )
   #    set( arg ${ARGV${index}} )
   #    set( str "${str}${arg}" )
   #    # set( str "${str} " )
   # endforeach( )

   foreach( arg ${ARGN} )
      set( str "${str}${arg}" )
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
