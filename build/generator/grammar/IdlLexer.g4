lexer grammar IdlLexer;

import FragmentsLexer ;



VERSION                       : 'version' ;
AUTHOR                        : 'author' ;
INTERFACE                     : 'interface' ;
METHOD                        : 'method' ;
REQUEST                       : 'request' ;
RESPONSE                      : 'response' ;
ATTRIBUTE                     : 'attribute' ;

LEFT_ROUND_BRACKET            : F_LEFT_ROUND_BRACKET ;
RIGHT_ROUND_BRACKET           : F_RIGHT_ROUND_BRACKET ;
LEFT_SQUARE_BRACKET           : F_LEFT_SQUARE_BRACKET ;
RIGHT_SQUARE_BRACKET          : F_RIGHT_SQUARE_BRACKET ;
LEFT_CURLY_BRACKET            : F_LEFT_CURLY_BRACKET ;
RIGHT_CURLY_BRACKET           : F_RIGHT_CURLY_BRACKET ;
DOT                           : F_DOT ;
COMMA                         : F_COMMA ;
SEMICOLON                     : F_SEMICOLON ;
COLON                         : F_COLON ;
ASTERISK                      : F_ASTERISK ;
SLASH                         : F_SLASH ;
BACK_SLASH                    : F_BACK_SLASH ;
PIPE                          : F_PIPE ;
PLUS                          : F_PLUS ;
MINUS                         : F_MINUS ;

SELECTOR                      : F_SELECTOR ;

WHITESPACE                    : (' ' | '\t')+ -> skip ;

NEWLINE                       : ('\r'? '\n' | '\r')+ -> skip ;

DIGIT                         : F_DIGIT ;

/*
DEC_DIGIT                     : F_DEC_DIGIT ;
HEX_DIGIT                     : F_HEX_DIGIT ;
OCT_DIGIT                     : F_OCT_DIGIT ;
BIN_DIGIT                     : F_BIN_DIGIT ;

DEC_NUMBER                    : F_DEC_DIGIT+ ([.,] F_DEC_DIGIT+)? ;
DEC_NUMBER_INT                : F_DEC_DIGIT+ ;
DEC_NUMBER_FLOAT              : F_DEC_DIGIT+ [.,] F_DEC_DIGIT+ ;

HEX_NUMBER_INT                : '0' [Xx] F_HEX_DIGIT+ ;

OCT_NUMBER_INT                : '0' F_OCT_DIGIT+ ;

BIN_NUMBER_INT                : F_BIN_DIGIT+ 'b' ;

VERSION_NUMBER                : F_DIGIT+ [.] F_DIGIT+ [.] F_DIGIT+ ;
*/

BLOCK_COMMENT                 : '/*' .*? '*/' ;
LINE_COMMENT                  : '//' ~[\r\n]* ('\r'? '\n' | '\r' | EOF) ;
COMMENT                       : (BLOCK_COMMENT | LINE_COMMENT) -> channel(HIDDEN) ;

IDENTIFIER                    : (F_LETTER | '_') (F_LETTER | F_DIGIT | '_')* ;

/*
 * TEXT                          : ('['|'(') .*? (']'|')') ;
 */


/*********************************************
 *
 * Nested curly braces
 * http://web.mit.edu/dmaze/school/6.824/antlr-2.7.0/doc/lexer.html
 *
 * SCOPE                         : '{' ( SCOPE | ~'}' )* '}' ;
 *********************************************/
