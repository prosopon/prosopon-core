%{

#include <stdio.h>

#include "gram.tab.h"


void slComment(void);
void mlComment(void);

void singleQuotedStringLiteral(char delimiter);
void tripleQuotedStringLiteral(char delimiter);

%}
 
digit         [0-9]
neg           \-
letter        [a-zA-Z_$]
hex_digit     [a-fA-F0-9]
E             [Ee][+-]?{digit}+
whitespace      [ \t\v\n\f]

%%

    /* Multi Line comments */
"\/\*"                      { mlComment(); }

    /* Single Line comments */
"//"                        { slComment(); }

    /* Triple Quoted String */
"\"\"\""                    { tripleQuotedStringLiteral('"'); return STRING_LITERAL; }
"\'\'\'"                    { tripleQuotedStringLiteral('\''); return STRING_LITERAL; }

    /* Single Quoted String */
"\""                        { singleQuotedStringLiteral('"'); return STRING_LITERAL; }
"\'"                        { singleQuotedStringLiteral('\''); return STRING_LITERAL; }

    /* Special Symbols */
"=>"                            { return ARROW_SYMBOL; }
"=:"                            { return DEFINE_SYMBOL; }

"|"                             { return BAR_SYMBOL; }
","                             { return VALUE_SEPARATOR; }

"["                             { return LIST_START_SYMBOL; }
"]"                             { return LIST_END_SYMBOL; }
"("                             { return TUPLE_START_SYMBOL; }
")"                             { return TUPLE_END_SYMBOL; }
"{"                             { return BLOCK_START_SYMBOL; }
"}"                             { return BLOCK_END_SYMBOL; }
"<"                             { return ACTOR_START_SYMBOL; }
">"                             { return ACTOR_END_SYMBOL; }

    /* Number Literals */
0[xX]{hex_digit}+                   { return NUMBER_LITERAL; }
{neg}?{digit}+{E}?                  { return NUMBER_LITERAL; }
{neg}?{digit}*"."{digit}+({E})?     { return NUMBER_LITERAL; }
{neg}?{digit}+"."{digit}*({E})?     { return NUMBER_LITERAL; }

    /* Identifier */
{letter}({letter}|{digit})+     { return IDENTIFIER; }


{whitespace}+                 /* skip whitespace */
.                           { }

%%

int yywrap(void)
{
    return 1;
 }

void slComment(void)
{
    char c;
    while ((c = yyinput()) && c != '\n')
        ;
    return;
}

void mlComment(void)
{
    char c = yyinput();
    if (!c)
        return;
        
    for (; c; c = yyinput())
    {
        if (c == '*')
        {
            char n = yyinput();
            if (n == '/')
                return;
        }
    }
}

void singleQuotedStringLiteral(char delimiter)
{
    char c, p = 0;
    while ((c = yyinput()) && !(c == delimiter && p != '\\'))
        p = c;
    return;
}

void tripleQuotedStringLiteral(char delimiter)
{
    char c;
    unsigned int count = 0;
    while ((c = yyinput()))
    {
        if (c == delimiter)
        {
            if (++count == 3)
                return;
        }
        else
            count = 0;
    }
    return;
}


