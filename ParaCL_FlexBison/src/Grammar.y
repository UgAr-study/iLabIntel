%{
    #include <iostream>
    #include <math>
    #include <vector>

    #include "../include/MainHeader.h"
%}

%union {
	std::string varname;
	int inumber;
}

%type <int> expr
%type <int> VAR
%token <int> INTEGER
%token VAR ERR SEMICOLON IF WHILE PRINT SCAN LESS OVER EQUAL NOTEQUAL LESSEQUAL OVEREQUAL OR AND
%token PLUS MINUS MULT DIV POW ASSIGN OROUNDBRAC CROUNDBRAC OBRACE CBRACE

%left '+' '-'
%left '*' '/'
%left NEGATE
%right '^'

%start prog

%%
prog :
	commands
	;

commands :
	|operation commands
	|command SEMICOLON commands
	;

command : func
	 |assign
	 ;

func :   PRINT expr { printf ("res is %d\n", $2); }
	|SCAN
	;

assign : VAR ASSIGN expr { $1 = $3; };

operation : IF OROUNDBRAC stms CROUNDBRAC OBRACE commands CBRACE
	   |WHILE OROUNDBRAC stms CROUNDBRAC OBRACE commands CBRACE
	  ;

stms :   stm
	|stm logop stms
	;

stm : expr compop expr;

compop:  LESS
	|OVER
	|EQUAL
	|NOTEQUAL
	|OVEREQUAL
	|LESSEQUAL
	;

logop :  OR
	|AND
	;

expr :   INTEGER	            { $$ = $1;      }
	|func			    { if ($1 == SCAN) {
				    	 scanf("%d", &$$);
				      }
				    }
	|VAR			    { $$ = $1;      }
	|expr PLUS expr             { $$ = $1 + $3; }
	|expr MINUS expr            { $$ = $1 - $3; }
	|expr MULT expr             { $$ = $1 * $3; }
	|expr DIV expr              { $$ = $1 / $3; }
	|MINUS expr %prec NEGATE    { $$ = -$2;     }
	|expr POW expr              { $$ = std::pow($1, $3); }
	|OROUNDBRAC expr CROUNDBRAC { $$ = $2;      }

%%