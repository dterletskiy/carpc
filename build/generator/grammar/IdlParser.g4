parser grammar IdlParser;

options { tokenVocab=IdlLexer; }



content              : element+ EOF ;

element              : ( author | interface ) ;

author               : AUTHOR IDENTIFIER ;

interface            : INTERFACE IDENTIFIER LEFT_CURLY_BRACKET ( version | ipc | request | response | attribute | method )* RIGHT_CURLY_BRACKET ;

version              : VERSION DIGIT+ DOT DIGIT+ DOT DIGIT+ ;

ipc                  : IPC COLON (TRUE|FALSE) SEMICOLON ;

/* request: int trigger_state( string state, size_t timeout ); */
request              : REQUEST COLON function SEMICOLON ;
/* response: int trigger_state( bool result ) -> trigger_state; */
response             : RESPONSE COLON function connection? SEMICOLON ;
connection           : SELECTOR IDENTIFIER ;

/* method: init( string command, size_t watchdog ) -> ( bool result ); */
method               : METHOD COLON procedure callback? SEMICOLON ;
callback             : SELECTOR arguments_list ;

/* attribute: string current_state; */
attribute            : ATTRIBUTE COLON IDENTIFIER IDENTIFIER SEMICOLON ;




procedure            : IDENTIFIER arguments_list ;
function             : IDENTIFIER IDENTIFIER arguments_list ;
arguments_list       : LEFT_ROUND_BRACKET arguments? RIGHT_ROUND_BRACKET ;
arguments_tuple      : LEFT_SQUARE_BRACKET arguments? RIGHT_SQUARE_BRACKET ;
arguments            : argument ( COMMA argument )* ;
argument             : IDENTIFIER IDENTIFIER ;
