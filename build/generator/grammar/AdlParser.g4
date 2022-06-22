parser grammar AdlParser;

import XdlParser;
options { tokenVocab=AdlLexer; }



content              : element+ EOF ;

element              : ( author | version | component | thread | application ) ;

component            : DEF_COMPONENT IDENTIFIER LEFT_CURLY_BRACKET ( include | creator )* RIGHT_CURLY_BRACKET SEMICOLON ;
include              : DEF_INCLUDE MODE_INCLUDE_QUOTES MODE_INCLUDE_PATH MODE_INCLUDE_QUOTES MODE_INCLUDE_SEMICOLON ;
creator              : DEF_CREATOR type SEMICOLON ;

thread               : DEF_THREAD IDENTIFIER LEFT_CURLY_BRACKET ( components | watchdog )* RIGHT_CURLY_BRACKET SEMICOLON ;
components           : DEF_COMPONENTS names SEMICOLON ;
watchdog             : DEF_WATCHDOG NUMBER SEMICOLON ;

application          : DEF_APPLICATION IDENTIFIER LEFT_CURLY_BRACKET ( threads )? RIGHT_CURLY_BRACKET SEMICOLON ;
threads              : DEF_THREADS names SEMICOLON ;
















/*
content              : element+ EOF ;

element              : ( author | application ) ;

author               : AUTHOR COLON IDENTIFIER SEMICOLON ;

version              : VERSION COLON NUMBER DOT NUMBER DOT NUMBER SEMICOLON ;

application          : APPLICATION IDENTIFIER LEFT_CURLY_BRACKET ( thread )* RIGHT_CURLY_BRACKET ;

thread               : THREAD IDENTIFIER LEFT_CURLY_BRACKET watchdog ( component )* RIGHT_CURLY_BRACKET ;

watchdog             : WATCHDOG COLON NUMBER ;

component            : COMPONENT IDENTIFIER LEFT_CURLY_BRACKET ( include | creator )* RIGHT_CURLY_BRACKET ;
include              : INCLUDE COLON QUOTES PATH QUOTES ;
creator              : CREATOR COLON function_name ;



function_name        : namespace_part? IDENTIFIER ;
namespace_part       : IDENTIFIER COLON COLON ;
*/
