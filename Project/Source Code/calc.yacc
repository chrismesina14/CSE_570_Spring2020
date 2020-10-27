/*
 * Name: Christian Mesina, Hugo Romero, Luis Escobar Urrutia
 * Class: CSE 570 Compilers
 * Instructor: Dr. Ernesto Gomez
 * Term: Spring 2020
**/

%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846
int regs[26];
double answer;
int yylex();
int yyerror(char *s);
int yywrap();
%}

%start list
%union {
  double a;
  char c;
}
%type <a> expr number DIGIT
%type <c> LETTER
%token DIGIT LETTER
%token QUIT
%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS

%%

list: /* empty */
    | list stat '\n'
    | list error '\n' {
      yyerrok;
    };

stat: expr {
        printf("%f\n", $1);
      }
    | LETTER '=' expr {
      regs[$1] = $3;
    };

expr: '(' expr ')' {
        $$ = $2;
        answer = $2;
      }
    | expr '*' expr {
        $$ = $1 * $3;
        answer = $$;
      }
    | '*' expr {
        $$ = answer * $2;
        answer = $$;
    }
    | expr '/' expr {
        $$ = $1 / $3;
        answer = $$;
      }
    | '/' expr {
        $$ = answer / $2;
        answer = $$;
      }
    | expr '+' expr {
        $$ = $1 + $3;
        answer = $$;
      }
    | '+' expr {
        $$ = answer + $2;
        answer = $$;
    }
    | expr '-' expr {
        $$ = $1 - $3;
        answer = $$;
      }
    | '-' expr {
        $$ = answer - $2;
        answer = $$;
    }
    | '-' expr %prec UMINUS {
        $$ = -$2;
        answer = $$;
      }
    | LETTER {
        $$ = regs[$1];
      }
    | number;
    | QUIT {
      return EXIT_SUCCESS;
    }

number: DIGIT {
          $$ = $1;
      };
%%

int main() {
  printf("Enter an expression:\n");
  return yyparse();  
}

int yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
  return 1;
}

int yywrap() {
  return 1;
}
