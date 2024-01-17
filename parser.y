/*
* @name parser.h
* @description parser of python
* @author fadel-hasan
*/

%code requires {
      #include "python_ast_node.hpp"
      #include <iostream>
      #include <string>
}

%union{
	AstNode* astNode;
        IdentifierNode* idNode;
	int d;
}

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
      AstNode* root = NULL;
      int n_nodes = 0;
%}

// tokens

%token<astNode> IF ELSE ELIF WHILE FOR CLASS AS IS ASSERT CONTINUE BREAK DEL EXCEPT IMPORT IN LAMBDA FINALLY GLOBAL NOT TRUE WITH YIELD FALSE AWAIT PASS RAISE NONE AND TRY FROM NONLOCAL ASYNC OR
%token<astNode> ID NUMBER STRING ASSIGN RETURN RANGE  
%token<astNode>  MUL  LBRACKET RBRACKET SEMICOLON EQUAL COLON
%token<astNode> PRINT KEYWORD IDENTIFIER DEF RSHIFT LSHIFT   
%token<astNode> INDENT DEDENT NEWLINE  NEQ  GT GTE LT  LTE MATCH CASE
%type<astNode> program statements statement function_def arg args args_ block function_call assignment
%type<astNode>  simple_stmt compound_stmt arguments argument global_stmt global_parms nonlocal_stmt nonlocal_parms
%type<astNode> yield_stmt yield_expr return_stmt return_parms while_stmt while_else with_stmt with_items
%type<astNode> with_item_list with_item if_stmt if_header elif_else_ elif_else else_stmt elif_stmts elif_stmt
%type<astNode> elif_header named_expression comparison assignment_expression comp_op decorators class_def class_def_raw
%type<astNode> primary_expression negated_expression expression for_stmt for_header changes range myfunc myrange try_stmt try_stmts
%type<astNode> except_block finally_block match_stmt match_cases match_case pattern_list  pattern list_pattern dict_pattern dict_pattern_entries dict_pattern_entry
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
program:  /*empty program*/ {$$ = nullptr;}
       | statements {      root = $$; YYACCEPT; }
       ;


statements: 
            statement  { $$ = new StatementsNode(); $$->add($1);}
          | statements statement  {$1->add($2); $$ = $1; }
          ;

statement: compound_stmt NEWLINE {$$=$1;}
         | simple_stmt NEWLINE {$$=$1;}
         /* | NEWLINE */
         ;


simple_stmt:
            expression   {{ $$ = $1; }}
          | assignment   {{ $$ = $1; }}
          | return_stmt  {{ $$ = $1; }}
          | BREAK        {{ $$ = new BreakStmtNode(); }}
          | CONTINUE     {{ $$ = new ContinueStmtNode(); }}
          | global_stmt  {{ $$ = $1; }}
          | nonlocal_stmt {{ $$ = $1; }}
          | yield_stmt    {{ $$ = $1; }}
          | PASS         {{ $$ = new PassStmtNode(); }}
          | function_call {{ $$ = $1; }}
          ;

compound_stmt:
     function_def {{ $$ = $1; }}
    | if_stmt      {{ $$ = $1; }}
    | class_def    {{ $$ = $1; }}
    | with_stmt    {{ $$ = $1; }}
    | for_stmt     {{ $$ = $1; }}
    | try_stmt     {{ $$ = $1; }}
    | while_stmt   {{ $$ = $1; }}
    | match_stmt   {{ $$ = $1; }}
    ;

function_def: DEF IDENTIFIER '(' args ')' COLON block {
      std::string name = "func" + std::to_string(n_nodes);
      ++n_nodes;
      IdentifierNode* idFunc = dynamic_cast<IdentifierNode*>($2);
      $$ = new FunctionNode(idFunc->value);
      $$->add($4);
      $$->add($7);
            };

args  : /* empty params */ {$$ = NULL;}
      | args_  {$$ = $1;}
      ;

args_ : arg { $$ = new Args("Args"); $$->add($1); }
      | args_ ',' arg { $1->add($3); $$ = $1; }
      ;

arg   : IDENTIFIER {
        std::string nname = "iden" + std::to_string(n_nodes);
        ++n_nodes;
        $1->name=nname;
        $$ = $1;
        }
      | NUMBER {
        std::string nname = "num" + std::to_string(n_nodes);
        ++n_nodes;
        $$ = new NumberNode(nname, $1);
      }
      ;

block : NEWLINE INDENT statements DEDENT { $$ = $3; }
    ;

function_call: IDENTIFIER '(' arguments ')' {   $$ = new FunctionCallNode($1);
      $$->add($3);}
             ;

arguments: /*empty*/ { $$ = new ArgumentsNode();}
         | arguments argument { $1->add($2);
                                    $$ = $1;}
         ;

argument:',' primary_expression {$$ = $2;}
        ;

global_stmt: GLOBAL IDENTIFIER global_parms {$$ = new GlobalStmtNode($2);
      if ($3) {
          for (const auto& param : $3->identifiers) {
              $$->add(param);
          }
      }}
           ;

global_parms: /*empty*/ { $$ = new GlobalStmtNode("");}
            /* | ',' IDENTIFIER  */
            |  global_parms ',' IDENTIFIER  { $1->add($3);
                                                 $$ = $1;}
            ;

nonlocal_stmt: NONLOCAL IDENTIFIER nonlocal_parms {$$ = new NonlocalStmtNode($2);
      if ($3) {
          for (const auto& param : $3->identifiers) {
              $$->add(param);
          }
      }}
             ;

nonlocal_parms: /*empty*/ { $$ = new NonlocalStatementNode("");}
              /* | ',' IDENTIFIER  */
              | nonlocal_parms ',' IDENTIFIER { $1->add($3);
                                                $$ = $1;}
              ;

yield_stmt: YIELD yield_expr {    $$ = new YieldStmtNode($2);}
          ;

yield_expr: expression {$$ = $1;}
          ;

assignment: IDENTIFIER ASSIGN expression  {$$ = new assignmentStatement("assign1");
                                          std::string nname = "iden" + std::to_string(n_nodes);
                                          ++n_nodes;
                                          $1->name=nname;
                                          $$->add($1);
                                          $$->add($3);}
          ;

return_stmt: RETURN return_parms {    $$ = new ReturnStatementNode($2);
}
           ;

return_parms:/*empty*/ {    $$ = nullptr; // No return value 
                        }
            | expression { $$ = $1;} 
            ;

while_stmt: WHILE comparison COLON block  while_else {    $$ = new WhileStatementNode($2, $4);
                                                        if ($5) {
                // Add the else statement to the while statement, or handle it as needed
                        $$->add($5);}}
          ;

while_else: /*empty*/ { $$ = nullptr;}
          | else_stmt {$$ = $1;}
          ;


with_stmt: WITH '(' with_items ')' COLON block {    $$ = new WithStmtNode($3, $6);}
         | WITH with_items COLON block {    $$ = new WithStmtNode($2, $4);}
         ;

with_items: with_item_list ',' {    $$ = $1;}
          | with_item_list {$$ = new WithItemsNode();
    $$->add($1);}
          ;

with_item_list: with_item {$$ = new WithItemList();
    $$->add($1);}
              | with_item_list ',' with_item { $1->add($3);
    $$ = $1;}
              ;

with_item: IDENTIFIER '(' STRING ')' AS IDENTIFIER { $$ = new WithItem($1, $3, $6);}
         ;




if_stmt : if_header block elif_else_ {    $$ = new IfStatementNode($1, $2, $3);}


if_header : IF named_expression COLON ; {    $$ = new IfHeaderNode($2);}


elif_else_ : /* empty no next elif or else*/ {$$ = nullptr;}
| elif_else {$$ = $1;}
 ; 


elif_else : elif_stmts else_stmt {$$ = new ElifElseNode($1, $2);}
| elif_stmts {$$ = new ElifElseNode($1, nullptr);}
| else_stmt { std::vector<AstNode*> emptyElifStmts;
    $$ = new ElifElseNode(emptyElifStmts, $1);}
;

else_stmt : ELSE COLON block {    $$ = new ElseStmtNode($3);}
;

elif_stmts : elif_stmt { $$ = new ElifStmtsNode();
    $$->add($1);}
| elif_stmt elif_stmts {$2->add($1);
    $$ = $2;}
;

elif_stmt : elif_header block {$$ = new ElifStmtNode($1, $2);}
;
elif_header : ELIF named_expression COLON { $$ = new ElifHeaderNode($2);}
;


named_expression: assignment_expression {$$ = $1;}
                | comparison {$$ = $1;}
    ;
    
comparison: expression comp_op expression {    $$ = new ComparisonNode($1, $2, $3);}
    ;



assignment_expression: IDENTIFIER ASSIGN expression {$$ = new AssignmentStatement($1);
    $$->add($3);}
    /* | conditional_expression */
    ;

comp_op: LT    { $$ = "<"; }
       | GT    { $$ = ">"; }
       | EQUAL { $$ = "=="; }
       | GTE   { $$ = ">="; }
       | LTE   { $$ = "<="; }
       | '<>'  { $$ = "<>"; }
       | NEQ   { $$ = "!="; }
       | IN    { $$ = "in"; }
       | NOT IN { $$ = "not in"; }
       | IS    { $$ = "is"; }
       /* | IS NOT { $$ = "is not"; } */
;


decorators: '@' named_expression NEWLINE decorators { $$ = new DecoratorsNode($2);
    $$->add($4);}
          | '@' named_expression NEWLINE { $$ = new DecoratorsNode($2);}
          ;

class_def: decorators class_def_raw {$$ = new ClassDefNode($1, $2);}
         | class_def_raw {$$ = new ClassDefNode(nullptr, $1);}
         ;

class_def_raw: CLASS IDENTIFIER COLON block; {$$ = new ClassDefRawNode($2, $4);}


primary_expression
  : IDENTIFIER {      $$ = new PrimaryExpressionNode($1);}
  | NUMBER {      $$ = new PrimaryExpressionNode(std::to_string($1));}
  | TRUE {      $$ = new PrimaryExpressionNode("true");}
  | FALSE {      $$ = new PrimaryExpressionNode("true");}
  
  ;

negated_expression
  : NOT primary_expression {      $$ = new NegatedExpressionNode($2);}
  ;

expression:   primary_expression {      $$ = $1;}
            | negated_expression {      $$ = $1;}
            | expression '+' expression {      $$ = new ExpressionNode("+", $1, $3);}
            | expression '-' expression {      $$ = new ExpressionNode("-", $1, $3);}
            | expression MUL expression {      $$ = new ExpressionNode("*", $1, $3);}
            | expression '/' expression {      $$ = new ExpressionNode("/", $1, $3);}
            | '-' expression  %prec UMINUS {      $$ = new ExpressionNode("-", nullptr, $2);}
            | '|' expression  %prec UMINUS {      $$ = new ExpressionNode("|", nullptr, $2);}
            | '(' expression ')' {      $$ = $2;}
;

for_stmt:  for_header changes COLON block {    $$ = new ForStatementNode($1, $2, $4);}

for_header: FOR IDENTIFIER IN {    $$ = new ForStatementNode($1, $2, $4);}

changes: IDENTIFIER {    $$ = new ChangesNode($1);}
        |range {$$ = new ChangesNode(""); // Assuming you want to handle range differently
    $$->add($1);}
        
range: RANGE '(' myrange ')' {$$ = $3;}
    | RANGE '(' myfunc ')' { $$ = $3; }
    
myfunc: IDENTIFIER '(' ')' {$$ = new MyFuncNode($1);}

myrange : NUMBER { std::vector<int> values =$1;
    $$ = new MyRangeNode(values);}
        | NUMBER ',' NUMBER {std::vector<int> values = { $1, $3 };
    $$ = new MyRangeNode(values);}
        | NUMBER ',' NUMBER ',' NUMBER {
            std::vector<int> values = { $1, $3, $5 };
                                        $$ = new MyRangeNode(values);}
        
        


try_stmt: TRY COLON block try_stmts {    $$ = new TryStatementNode($3, $4);}
;

try_stmts: except_block {  $$ = new TryStmtsNode();
    $$->add($1);}
         | else_stmt {$$ = new TryStmtsNode();
    $$->add($1);}
         | finally_block {$$ = new TryStmtsNode();
    $$->add($1);}
         | try_stmts finally_block { $1->add($2);
    $$ = $1;}
         ;

except_block
    : EXCEPT IDENTIFIER COLON block {    $$ = new ExceptBlockNode($2, $4);}
    | except_block EXCEPT IDENTIFIER COLON block { $$ = $1;
    $$->add(new ExceptBlockNode($3, $5));}
    ;

finally_block:FINALLY COLON block {    $$ = new FinallyBlockNode($3);}
; 



match_stmt
    : MATCH expression COLON match_cases {    $$ = new MatchStmtNode($2, $4);}
    ;

match_cases
    : match_case {$$ = new MatchCasesNode();
    $$->add($1);}
    | match_cases match_case { $1->add($2);
    $$ = $1;}
    ;

match_case
    : CASE pattern_list COLON simple_stmt {    $$ = new MatchCaseNode($2, $4);
}
    ;

pattern_list
    : pattern { $$ = new PatternListNode();
    $$->add($1);}
    | pattern_list ',' pattern { $1->add($3);
    $$ = $1;}
    ;

    
pattern:
      expression {    $$ = new PatternNode($1);
}
    /* | tuple_pattern */
    | list_pattern {        $$ = $1;

}
    | dict_pattern {    $$ = $1;}
    | '_' {    $$ = new PatternNode(new LiteralNode("_"));}
    ;

/* tuple_pattern: '(' pattern_list ')'
    ; */

list_pattern
    : '[' pattern_list ']' {    $$ = new ListPatternNode($2);}
    ;

dict_pattern
    : '{' dict_pattern_entries '}' {    $$ = new DictPatternNode($2);
}
    ;

dict_pattern_entries
    : dict_pattern_entry  {$$ = new DictPatternEntriesNode();
    $$->add($1);}
    | dict_pattern_entries ',' dict_pattern_entry { $1->add($3);
    $$ = $1;}
    ;

dict_pattern_entry
    : pattern COLON pattern {    $$ = new DictPatternEntryNode($1, $3);}
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
      if (root != NULL) {
            AST ast(root);
            ast.Print();
      }
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