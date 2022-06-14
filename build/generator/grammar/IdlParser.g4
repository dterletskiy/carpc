parser grammar IdlParser;

import XdlParser;
options { tokenVocab=IdlLexer; }



content              : element+ EOF ;

element              : ( author | version | import_xdl | interface | ipc | request | response | attribute | method ) ;

import_xdl           : DEF_IMPORT MODE_IMPORT_QUOTES MODE_IMPORT_PATH MODE_IMPORT_QUOTES MODE_IMPORT_SEMICOLON ;

interface            : DEF_INTERFACE IDENTIFIER SEMICOLON ;

ipc                  : DEF_IPC (TRUE|FALSE) SEMICOLON ;

// request: int trigger_state( string state, size_t timeout );
request              : DEF_REQUEST function SEMICOLON ;
// response: int trigger_state( bool result ) -> trigger_state;
response             : DEF_RESPONSE function connection? SEMICOLON ;
connection           : SELECTOR IDENTIFIER ;

// method: init( string command, size_t watchdog ) -> ( bool result );
method               : DEF_METHOD procedure callback? SEMICOLON ;
callback             : SELECTOR arguments_list ;

// attribute: string current_state;
attribute            : DEF_ATTRIBUTE IDENTIFIER IDENTIFIER SEMICOLON ;
