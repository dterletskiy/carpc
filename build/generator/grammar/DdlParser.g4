parser grammar DdlParser;

import XdlParser;
options { tokenVocab=DdlLexer; }



content              : element+ EOF ;

element              : ( author | version | data | struct ) ;

data                 : DEF_DATA IDENTIFIER SEMICOLON ;

struct               : DEF_STRUCT IDENTIFIER LEFT_CURLY_BRACKET ( field )* RIGHT_CURLY_BRACKET SEMICOLON ;

method               : DEF_METHOD function SEMICOLON ;

field                : DEF_FIELD argument SEMICOLON ;

