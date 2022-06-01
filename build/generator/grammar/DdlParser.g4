parser grammar DdlParser;

import Parser;
options { tokenVocab=DdlLexer; }



content              : element+ EOF ;

element              : ( author | data ) ;

data                 : DATA IDENTIFIER LEFT_CURLY_BRACKET ( version | struct )* RIGHT_CURLY_BRACKET ;


struct               : STRUCT IDENTIFIER LEFT_CURLY_BRACKET ( field )* RIGHT_CURLY_BRACKET SEMICOLON ;

method               : METHOD COLON function SEMICOLON ;

field                : FIELD COLON argument SEMICOLON ;

