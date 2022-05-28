parser grammar Parser;

options { tokenVocab=DdlLexer; }



author               : AUTHOR IDENTIFIER ;

version              : VERSION DIGIT+ DOT DIGIT+ DOT DIGIT+ SEMICOLON ;



procedure            : IDENTIFIER arguments_list ;
function             : IDENTIFIER IDENTIFIER arguments_list ;
arguments_list       : LEFT_ROUND_BRACKET arguments? RIGHT_ROUND_BRACKET ;
arguments_tuple      : LEFT_SQUARE_BRACKET arguments? RIGHT_SQUARE_BRACKET ;
arguments            : argument ( COMMA argument )* ;
argument             : type name ;
type                 : ( IDENTIFIER | IDENTIFIER LESS IDENTIFIER GREATER ) AMPERSAND? ;
name                 : IDENTIFIER ;
