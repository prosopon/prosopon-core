%{
#include <stdio.h>

#define YYSTYPE int

#define YYDEBUG 1


int yylex();
int yyerror(const char *p);

extern int yydebug;

%}

%token IDENTIFIER
%token STRING_LITERAL
%token NUMBER_LITERAL

%token LIST_START_SYMBOL
%token LIST_END_SYMBOL
%token TUPLE_START_SYMBOL
%token TUPLE_END_SYMBOL
%token ACTOR_START_SYMBOL
%token ACTOR_END_SYMBOL
%token BLOCK_START_SYMBOL
%token BLOCK_END_SYMBOL

%token BAR_SYMBOL
%token ARROW_SYMBOL
%token DEFINE_SYMBOL
%token VALUE_SEPARATOR

%start program

%%

/**
 * Values
 */
 
simple_value
    : IDENTIFIER
    | STRING_LITERAL
    | NUMBER_LITERAL
    ;
    
value
    : simple_value
    | list
    | tuple
    | block
    | actor
    ;

value_list
    : value_list_item
    | value_list value_list_item
    ;
    
value_list_item
    : value
    | value VALUE_SEPARATOR
    ;
    

/**
 * Definition
 */
 
definition
    : value_definition
    | list_reduction_definition
    ;
 
value_definition
    : IDENTIFIER DEFINE_SYMBOL value
    ;
 
list_reduction_definition
    : list ARROW_SYMBOL value
    ;
 
definition_list
    : definitions BAR_SYMBOL
    ;

definitions
    : definition
    | definitions definition
    ; 

  

/**
 * List
 */

list
    : LIST_START_SYMBOL LIST_END_SYMBOL
    | LIST_START_SYMBOL list_body LIST_END_SYMBOL
    ;
    
list_body
    : IDENTIFIER
    | IDENTIFIER value_list
    | definition_list IDENTIFIER
    | definition_list IDENTIFIER value_list
    ;




/**
 * Tuple
 */

tuple
    : TUPLE_START_SYMBOL TUPLE_END_SYMBOL
    | TUPLE_START_SYMBOL tuple_body TUPLE_END_SYMBOL
    ;
    
tuple_body
    : value_list
    | definition_list value_list
    ;


/**
 * Block
 */
    
block
    : BLOCK_START_SYMBOL BLOCK_END_SYMBOL
    | BLOCK_START_SYMBOL block_body BLOCK_END_SYMBOL
    ;

block_body
    : value_list
    | definition_list value_list
    ;

/**
 * Actor
 */    

actor
    : ACTOR_START_SYMBOL ACTOR_END_SYMBOL
    | ACTOR_START_SYMBOL actor_body ACTOR_END_SYMBOL
    ;

actor_body
    : actor_items
    | definition_list actor_items
    ;

actor_items
    : actor_item
    | actor_items actor_item
    ;
    
actor_item
    : value value
    ; 
    

/**
 * Main Program
 */

program
    : statement_list
    | empty_statement
    ;

statement_list
    : statement_list statement
    | statement
    ;
    
statement
    : value
    | definition
    ;
    
empty_statement
    :
    ;


    
    
%%

int yyerror(const char *p)
{
    printf("\nError! %s\n", p);
}


int main()
{
    yydebug = 1;
    yyparse();
    return 0;
}

