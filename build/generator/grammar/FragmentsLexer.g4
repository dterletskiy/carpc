lexer grammar FragmentsLexer;



fragment F_Aa                    : ('A'|'a') ;
fragment F_Bb                    : ('B'|'b') ;
fragment F_Cc                    : ('C'|'c') ;
fragment F_Dd                    : ('D'|'d') ;
fragment F_Ee                    : ('E'|'e') ;
fragment F_Ff                    : ('F'|'f') ;
fragment F_Gg                    : ('G'|'g') ;
fragment F_Hh                    : ('H'|'h') ;
fragment F_Ii                    : ('I'|'i') ;
fragment F_Jj                    : ('J'|'j') ;
fragment F_Kk                    : ('K'|'k') ;
fragment F_Ll                    : ('L'|'l') ;
fragment F_Mm                    : ('M'|'m') ;
fragment F_Nn                    : ('N'|'n') ;
fragment F_Oo                    : ('O'|'o') ;
fragment F_Pp                    : ('P'|'p') ;
fragment F_Qq                    : ('Q'|'q') ;
fragment F_Rr                    : ('R'|'r') ;
fragment F_Ss                    : ('S'|'s') ;
fragment F_Tt                    : ('T'|'t') ;
fragment F_Uu                    : ('U'|'u') ;
fragment F_Vv                    : ('V'|'v') ;
fragment F_Ww                    : ('W'|'w') ;
fragment F_Xx                    : ('X'|'x') ;
fragment F_Yy                    : ('Y'|'y') ;
fragment F_Zz                    : ('Z'|'z') ;

fragment F_DIGIT                 : [0-9] ;
fragment F_DEC_DIGIT             : [0-9] ;
fragment F_HEX_DIGIT             : [0-9A-Fa-f];
fragment F_OCT_DIGIT             : [0-7];
fragment F_BIN_DIGIT             : [01];

fragment F_LEFT_ROUND_BRACKET    : '(' ;
fragment F_RIGHT_ROUND_BRACKET   : ')' ;
fragment F_LEFT_SQUARE_BRACKET   : '[' ;
fragment F_RIGHT_SQUARE_BRACKET  : ']' ;
fragment F_LEFT_CURLY_BRACKET    : '{' ;
fragment F_RIGHT_CURLY_BRACKET   : '}' ;
fragment F_DOT                   : '.' ;
fragment F_COMMA                 : ',' ;
fragment F_SEMICOLON             : ';' ;
fragment F_COLON                 : ':' ;
fragment F_QUOTES                : '"' ;
fragment F_APOSTROPHE            : '\'' ;
fragment F_BACK_APOSTROPHE       : '`' ;
fragment F_ASTERISK              : '*' ;
fragment F_SLASH                 : '/' ;
fragment F_BACK_SLASH            : '\\' ;
fragment F_PIPE                  : '|' ;
fragment F_PLUS                  : '+' ;
fragment F_MINUS                 : '-' ;
fragment F_UNDERSCORE            : '_' ;
fragment F_EQUAL                 : '=' ;
fragment F_LESS                  : '<' ;
fragment F_GREATER               : '>' ;
fragment F_AMPERSAND             : '&' ;
fragment F_BLOCK                 : '█' ;
fragment F_TOP_HALF_BLOCK        : '▀' ;
fragment F_BOTTOM_HALF_BLOCK     : '▄' ;

fragment F_LETTER_LOWERCASE      : [a-z] ;
fragment F_LETTER_UPPERCASE      : [A-Z] ;
fragment F_LETTER                : F_LETTER_LOWERCASE | F_LETTER_UPPERCASE ;

fragment F_TRUE                  : 'true' ;
fragment F_FALSE                 : 'false' ;

fragment F_SELECTOR              : '->' ;
