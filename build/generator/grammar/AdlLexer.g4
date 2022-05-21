lexer grammar AdlLexer;

import FragmentsLexer ;



VERSION                       : 'version' ;
AUTHOR                        : 'author' ;
COMPONENT                     : 'component' ;
COMPONENTS                    : 'components' ;
INCLUDE                       : 'include' ;
CREATOR                       : 'creator' ;
THREAD                        : 'thread' ;
THREADS                       : 'threads' ;
WATCHDOG                      : 'watchdog' ;
APPLICATION                   : 'application' ;

TRUE                          : F_TRUE ;
FALSE                         : F_FALSE ;

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
QUOTES                        : F_QUOTES ;
ASTERISK                      : F_ASTERISK ;
SLASH                         : F_SLASH ;
BACK_SLASH                    : F_BACK_SLASH ;
PIPE                          : F_PIPE ;
PLUS                          : F_PLUS ;
MINUS                         : F_MINUS ;

SELECTOR                      : F_SELECTOR ;

WHITESPACE                    : (' ' | '\t')+ -> skip ;

NEWLINE                       : ('\r'? '\n' | '\r')+ -> skip ;

NUMBER                        : F_DIGIT+ ;

BLOCK_COMMENT                 : '/*' .*? '*/' ;
LINE_COMMENT                  : '//' ~[\r\n]* ('\r'? '\n' | '\r' | EOF) ;
COMMENT                       : (BLOCK_COMMENT | LINE_COMMENT) -> channel(HIDDEN) ;

IDENTIFIER                    : (F_LETTER | F_UNDERSCORE) (F_LETTER | F_DIGIT | F_UNDERSCORE)* ;

/*
NAMESPACE_IDENTIFIER          : (F_LETTER | F_UNDERSCORE | '::') (F_LETTER | F_DIGIT | F_UNDERSCORE | '::')* ;
*/
NAMESPACE_IDENTIFIER          : '::'? (IDENTIFIER '::')* IDENTIFIER ;

PATH                          : (F_LETTER | F_DIGIT | F_UNDERSCORE | F_DOT | F_SLASH)+ ;
