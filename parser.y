/*
* @name parser.h
* @description parser of python
* @author fadel-hasan
*/
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int yydebug=1;
FILE *yyin;
void yyerror(const char *);
extern int yylex();
extern int yylineno;
extern char* yytext;
%}

// tokens

%token IF ELSE ELIF WHILE FOR CLASS AS IS ASSERT CONTINUE BREAK DEL EXCEPT IMPORT IN LAMBDA FINALLY GLOBAL NOT TRUE WITH YIELD FALSE AWAIT PASS RAISE NONE AND TRY FROM NONLOCAL ASYNC OR
%token ID NUMBER STRING ASSIGN RETURN RANGE  
%token  MUL  LBRACKET RBRACKET SEMICOLON EQUAL COLON
%token PRINT KEYWORD IDENTIFIER DEF RSHIFT LSHIFT   
%token INDENT DEDENT NEWLINE  NEQ  GT GTE LT  LTE MATCH CASE

%nonassoc EQUAL
%left '+' '-'
%left MUL '/'
%left '(' '[' '{'
%right "**"
%nonassoc '|' UMINUS

%start program

%%
/* program: 
|         write yyaccept          */
/* Parser Grammar */
program:  /*empty program*/
       | statements {YYACCEPT;}
       ;


statements: 
            statement  {printf("one statment\n");}
          | statements statement  {printf("many statment\n");}
          ;

statement: compound_stmt NEWLINE {printf("compound stmt\n");}
         | simple_stmt NEWLINE {printf("simple stmt\n");}
         /* | NEWLINE */
         ;


simple_stmt:  expression   {printf("expression\n");}
            | assignment   {printf("assignment\n");}
            | return_stmt  {printf("return stmt\n");}
            | BREAK        {printf("break stmt\n");}
            | CONTINUE     {printf("continue stmt\n");}
            | global_stmt  {printf("global stmt\n");}
            | nonlocal_stmt {printf("nonlocal stmt\n");}
            | yield_stmt    {printf("yield stmt\n");}
            | PASS         {printf("pass\n");}
            | function_call {printf("function call\n");}
            ;

compound_stmt
    : function_def {printf("function def\n");}
    | if_stmt      {printf("if stmt\n");}
    | class_def    {printf("class def\n");}
    | with_stmt    {printf("with stmt\n");}
    | for_stmt     {printf("for stmt\n");}
    | try_stmt     {printf("try stmt\n");}
    | while_stmt   {printf("while stmt\n");}
    | match_stmt   {printf("match stmt\n");}
    ; 

function_def: DEF IDENTIFIER '(' args ')' COLON block {
            printf("Function successfully parsed:\n"); 
            };

args  : /* empty params */
      | args_  
      ;

args_ : arg 
      | args_ ',' arg 
      ;

arg   : IDENTIFIER 
      | NUMBER 
      ;

block : NEWLINE INDENT statements DEDENT
    ;

function_call: IDENTIFIER '(' arguments ')'
             ;

arguments: /*empty*/ 
         | arguments argument
         ;

argument:',' primary_expression
        ;

global_stmt: GLOBAL IDENTIFIER global_parms
           ;

global_parms: /*empty*/ 
            /* | ',' IDENTIFIER  */
            |  global_parms ',' IDENTIFIER 
            ;

nonlocal_stmt: NONLOCAL IDENTIFIER nonlocal_parms
             ;

nonlocal_parms: /*empty*/ 
              /* | ',' IDENTIFIER  */
              | nonlocal_parms ',' IDENTIFIER
              ;

yield_stmt: YIELD yield_expr
          ;

yield_expr: expression 
          ;

assignment: IDENTIFIER ASSIGN expression  
          ;

return_stmt: RETURN return_parms 
           ;

return_parms:/*empty*/ 
            | expression 
            ;

while_stmt: WHILE comparison COLON block  while_else
          ;

while_else: /*empty*/
          | else_stmt
          ;
with_stmt: WITH '(' with_items ')' COLON block
         | WITH with_items COLON block
         ;

with_items: with_item_list ','
          | with_item_list
          ;

with_item_list: with_item
              | with_item_list ',' with_item
              ;

with_item: IDENTIFIER '(' STRING ')' AS IDENTIFIER 
         ;




if_stmt : if_header block elif_else_


if_header : IF named_expression COLON ;


elif_else_ : /* empty no next elif or else*/
| elif_else ;


elif_else : elif_stmts else_stmt
| elif_stmts
| else_stmt
;

else_stmt : ELSE COLON block
;

elif_stmts : elif_stmt
| elif_stmt elif_stmts
;

elif_stmt : elif_header block
;
elif_header : ELIF named_expression COLON
;


named_expression: assignment_expression
                | comparison
    ;
    
comparison: expression comp_op expression
    ;



assignment_expression: IDENTIFIER ASSIGN expression
    /* | conditional_expression */
    ;


comp_op: LT
    | GT
    | EQUAL
    | GTE
    | LTE
    | '<>'
    | NEQ
    | IN
    | NOT IN
    | IS
    /* | IS NOT  */
    ;


decorators: '@' named_expression NEWLINE decorators
          | '@' named_expression NEWLINE
          ;

class_def: decorators class_def_raw
         | class_def_raw
         ;

class_def_raw: CLASS IDENTIFIER COLON block;


primary_expression
  : IDENTIFIER
  | NUMBER
  | TRUE
  | FALSE
  
  ;

negated_expression
  : NOT primary_expression
  ;

expression:   primary_expression
            | negated_expression
            | expression '+' expression
            | expression '-' expression
            | expression MUL expression
            | expression '/' expression
            | '-' expression  %prec UMINUS 
            | '|' expression  %prec UMINUS
            | '(' expression ')'
;

for_stmt:  for_header changes COLON block

for_header: FOR IDENTIFIER IN 

changes: IDENTIFIER
        |range
        
range: RANGE '(' myrange ')'
    | RANGE '(' myfunc ')'
    
myfunc: IDENTIFIER '(' ')'

myrange : NUMBER 
        | NUMBER ',' NUMBER
        | NUMBER ',' NUMBER ',' NUMBER
        
        


try_stmt: TRY COLON block try_stmts;

try_stmts: except_block
         | else_stmt
         | finally_block
         | try_stmts finally_block
         ;

except_block
    : EXCEPT IDENTIFIER COLON block
    | except_block EXCEPT IDENTIFIER COLON block
    ;

finally_block:FINALLY COLON block;



match_stmt
    : MATCH expression COLON match_cases
    ;

match_cases
    : match_case
    | match_cases match_case
    ;

match_case
    : CASE pattern_list COLON simple_stmt
    ;

pattern_list
    : pattern 
    | pattern_list ',' pattern
    ;

    
pattern:
      expression
    /* | tuple_pattern */
    | list_pattern
    | dict_pattern
    | '_'
    ;

/* tuple_pattern: '(' pattern_list ')'
    ; */

list_pattern
    : '[' pattern_list ']'
    ;

dict_pattern
    : '{' dict_pattern_entries '}'
    ;

dict_pattern_entries
    : dict_pattern_entry 
    | dict_pattern_entries ',' dict_pattern_entry
    ;

dict_pattern_entry
    : pattern COLON pattern
    ; 

/* expression
    : expression '+' expression     
    | expression '-' expression     
    | expression MUL expression     
    | expression '/' expression     
    | '-' expression  %prec UMINUS  
    | '(' expression ')'            
    | atom
                     
 ; */

/* 
if_stmt: IF named_expression COLON block elif_stmt 
       | IF named_expression COLON block else_block
       ;

elif_stmt:
    | ELIF named_expression COLON block else_block 
    | elif_stmt ELIF named_expression COLON block else_block
    ;

else_block:
    | ELSE COLON block 
    ; */
/* atom:
     IDENTIFIER
    | NUMBER
    | STRING
    | NONE
    | TRUE
    | FALSE
    /* | '(' expression ')' 
    ;
    
*/


/* 
conditional_expression: or_expression
    | conditional_expression or_expression
    ;

or_expression: and_expression
    | or_expression OR and_expression
    ;

and_expression: not_expression
    | and_expression AND not_expression
    ;

not_expression: not_expression NOT 
              | comparison
    ; */ 


/* condition: expression
  | condition AND condition 
  | condition OR condition
    ; */

/* while_statement
  : WHILE conditional_expression COLON  block
  ; */ 

/* 

for_stmt: FOR expression_list IN expression_list COLON block
    ;
    
expression_list
    : star_expression
    | expression_list ',' star_expression
    ;

star_expression
    : primary_expression
    | unary_expression
    | binary_expression
    | call_expression
    | attribute_expression
    | subscript_expression
    | conditional_expression
    | lambda_expression
    ;

primary_expression
    : atom
    ;

unary_expression
    : '-' star_expression
    | NOT star_expression
    ;

binary_expression
    : star_expression '+' star_expression
    | star_expression '-' star_expression
    | star_expression MUL star_expression
    | star_expression '/' star_expression
    | star_expression EQUAL star_expression
    | star_expression '!=' star_expression
    | star_expression LESS_THAN star_expression
    | star_expression GREATER_THAN star_expression
    | star_expression LESS_THAN_EQUAL star_expression
    | star_expression '>=' star_expression
    ;

call_expression
    : IDENTIFIER '(' arguments ')'
    ;

attribute_expression
    : star_expression '.' IDENTIFIER
    ;

subscript_expression
    : star_expression '[' star_expression ']'
    ;

lambda_expression
    : LAMBDA parameter_list COLON star_expression
    ;

parameter_list
    : IDENTIFIER 
    | parameter_list ',' IDENTIFIER
    ;

match_stmt
    : 'match' star_expression COLON match_cases
    ;

match_cases
    : match_case
    | match_cases match_case
    ;

match_case
    : pattern_list '=>' block
    ;

pattern_list
    : pattern 
    | pattern_list ',' pattern
    ;

pattern
    : expression
    | tuple_pattern
    | list_pattern
    | dict_pattern
    | IDENTIFIER
    ;

tuple_pattern
    : '(' pattern_list ')'
    ;

list_pattern
    : '[' pattern_list ']'
    ;

dict_pattern
    : '{' dict_pattern_entries '}'
    ;

dict_pattern_entries
    : dict_pattern_entry 
    | dict_pattern_entries ',' dict_pattern_entry
    ;

dict_pattern_entry
    : pattern COLON pattern
    ; */
%%


int main(int argc, char **argv)
{
 /*success("This is a valid python expression");*/
     if (argc > 1){
        for(int i=0;i<argc;i++)
            printf("value of argv[%d] = %s\n\n",i,argv[i]);
            yyin=fopen(argv[1],"r");
    }
        else
        yyin=stdin;
     yyparse();
     return 0;
}

/* int yyerror(const char* s) {
//     fprintf(stderr, "Error: %s\n", s);
//     return 1;
// }*/

/* void yyerror(const char *msg)
    {
          printf(" %s \n", msg);
    } */

    void yyerror(const char* s){
    fprintf(stderr, "%s \n", s);
    fprintf(stderr, "line %d: ", yylineno);
    fprintf(stderr, "%s \n", yytext);
    exit(1);
}