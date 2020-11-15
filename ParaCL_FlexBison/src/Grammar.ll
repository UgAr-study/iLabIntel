%option c++

%{
    #include "Grammar.tab.hh"
%}

WS [ \t\n\v]+
DIGIT [0-9]
DIGIT1 [1-9]
VARS   [a-zA-z][a-zA-Z0-9_]*

%%

{WS}              /* nothing */
"+"               return PLUS;
"-"               return MINUS;
"*"               return MULT;
"/"               return DIV;
"^"               return POW;
"="               return ASSIGN;
"("               return OROUNDBRAC;
")"               return CROUNDBRAC;
"{"               return OBRACE;
"}"               return CBRACE;
";"               return SEMICOLON;
{DIGIT1}{DIGIT}*  { yylval = atoi(yytext); return INTEGER; }
{VARS}            { yylval = strdup(yytext); return VAR; }
"print"           return PRINT;
"?"               return SCAN;
"if"              return IF;
"while"           return WHILE;
"||"              return OR;
"&&"              return AND;
"=="              return EQUAL;
"!="              return NOTEQUAL;
"<"               return LESS;
">"               return OVER;
"<="              return LESSEQUAL;
">="              return OVEREQUAL;
.			      return ERR;

%%
