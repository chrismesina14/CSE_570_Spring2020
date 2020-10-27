/*
 * Name: Christian Mesina, Hugo Romero, Luis Escobar Urrutia
 * Class: CSE 570 Compilers
 * Instructor: Dr. Ernesto Gomez
 * Term: Spring 2020
**/

%{
#include <stdio.h>
#include "y.tab.h"

int c;
double d;
extern YYSTYPE yylval;
%}

%%

" ";


[a-z] {
  c = yytext[0];
  yylval.a = c - 'a';
  return(LETTER);
}

[0-9] {
  d = atof(yytext);
  yylval.a = d - 0;
  return(DIGIT);
}

[^a-z0-9\b] {
  c = yytext[0];
  return(c);
}

[0-9]+\.[0-9]+ {
  d = atof(yytext);
  yylval.a = d - 0;
  return(DIGIT);
}

QUIT {
  return QUIT;
}

