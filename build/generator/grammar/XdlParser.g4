parser grammar XdlParser;

options { tokenVocab=XdlLexer; }



author               : DEF_AUTHOR IDENTIFIER SEMICOLON ;

version              : DEF_VERSION version_number SEMICOLON ;
version_number       : NUMBER DOT NUMBER DOT NUMBER ;



procedure            : name arguments_list ;
function             : type name arguments_list ;
arguments_list       : LEFT_ROUND_BRACKET arguments? RIGHT_ROUND_BRACKET ;
arguments_tuple      : LEFT_SQUARE_BRACKET arguments? RIGHT_SQUARE_BRACKET ;
arguments            : argument ( COMMA argument )* ;
argument             : type name ;
type                 : NAMESPACE? ( IDENTIFIER | IDENTIFIER LESS IDENTIFIER GREATER ) AMPERSAND? ;
names                : name ( COMMA name )* ;
name                 : IDENTIFIER ;
