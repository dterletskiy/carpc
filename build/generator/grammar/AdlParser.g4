parser grammar AdlParser;

options { tokenVocab=AdlLexer; }



/*
content              : element+ EOF ;

element              : ( author | application ) ;

author               : AUTHOR IDENTIFIER ;

application          : APPLICATION IDENTIFIER LEFT_CURLY_BRACKET ( thread )* RIGHT_CURLY_BRACKET ;

thread               : THREAD IDENTIFIER LEFT_CURLY_BRACKET watchdog ( component )* RIGHT_CURLY_BRACKET ;

watchdog             : WATCHDOG COLON NUMBER ;

component            : COMPONENT IDENTIFIER LEFT_CURLY_BRACKET ( include | creator )* RIGHT_CURLY_BRACKET ;
include              : INCLUDE COLON QUOTES PATH QUOTES ;
creator              : CREATOR COLON function_name ;



function_name        : namespace_part? IDENTIFIER ;
namespace_part       : IDENTIFIER COLON COLON ;
*/



content              : element+ EOF ;

element              : ( author | component | thread | application ) ;

author               : AUTHOR IDENTIFIER ;

component            : COMPONENT IDENTIFIER LEFT_CURLY_BRACKET ( include | creator )* RIGHT_CURLY_BRACKET ;
include              : INCLUDE COLON QUOTES PATH QUOTES ;
creator              : CREATOR COLON NAMESPACE_IDENTIFIER ;

thread               : THREAD IDENTIFIER LEFT_CURLY_BRACKET ( components | watchdog )* RIGHT_CURLY_BRACKET ;
components           : COMPONENTS COLON arguments ;
watchdog             : WATCHDOG COLON NUMBER ;

application          : APPLICATION IDENTIFIER LEFT_CURLY_BRACKET ( threads ) RIGHT_CURLY_BRACKET ;
threads              : THREADS COLON arguments ;



function_name        : namespace_part? IDENTIFIER ;
namespace_part       : IDENTIFIER COLON COLON ;
arguments            : argument ( COMMA argument )* ;
argument             : IDENTIFIER ;
