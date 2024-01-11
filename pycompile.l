/*
* @name parser.h
* @description lexer of python
* @author fadel-hasan
*/
/*
To prevent the yywrap() method from being created
 because we don't need to execute anything at the end of the file*/
%option noyywrap
%option yylineno
/*
create STRING & STRING2 to work with "" or ''
STRING to work with string in ""
STRING2 to work with string in ''
STRING3 to work with string """ """
COMMENT to work with multiable commnt line like """ """
*/
%x STRING1
%x STRING2
%x STRING3
%x COMMENT


/*
Libraries needed to work with C language
*/
%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "stack.c"
#include "parser.tab.h"
%}

/* Define token types as constants */
%{
#define IDENTI 1
#define KEYWORDS 2
#define OPERATOR 4
#define Delimiters 5
char* string_literal_value = NULL;
int lno=1;
struct StackNode* myStack = NULL;
%}

/* Regular expressions to match tokens */
DIGIT [0-9]
LETTER [_a-zA-Z]
ALPHANUM {LETTER}|{DIGIT}
IDENTI {LETTER}{ALPHANUM}*


/*
integer :
example for this matched case :
7     2147483647                        0o177    0b100110111
3     79228162514264337593543950336     0o377    0xdeadbeef
      100_000_000_000                   0b_1110_0101

 floatnumber:
 example for this matched case :
3.14    10.    .001    1e100    3.14e-10    0e0    3.14_15_93

number :
 example for this matched case :

intger or folat
*/
NUMBER {integer}|{floatnumber}
integer {decinteger}|{bininteger}|{octinteger}|{hexinteger}
decinteger   ({nonzerodigit})(_?{DIGIT})*|(0)+(_?0)*
bininteger   0[b|B]((_?{bindigit})+)
octinteger   0[o|O]((_?{octdigit})+)
hexinteger   0[x|X]((_?{hexdigit})+)
nonzerodigit [1-9]
bindigit     0|1
octdigit     [0-7]
hexdigit     {DIGIT}|[a-fA-F]


floatnumber   ({pointfloat}|{exponentfloat})
pointfloat    ({digitpart}?{fraction}|{digitpart}"\.")
exponentfloat ({digitpart}|{pointfloat}){exponent}
digitpart     {DIGIT}+(["_"]{DIGIT}+)*
fraction      "\."{digitpart}
exponent      ["e"|"E"][+|-]?{digitpart}

/*
keyword :
it is match all keywords in python:
False      await      else       import     pass
None       break      except     in         raise
True       class      finally    is         return
and        continue   for        lambda     try
as         def        from       nonlocal   while
assert     del        global     not        with
async      elif       if         or         yield
*/

KEYWORDS if|else|elif|while|for|def|class|as|is|assert|continue|break|del|except|import|in|lambda|print|finally|global|not|return|True|with|yield|False|await|pass|raise|None|and|try|from|nonlocal|async|or

/*
operators:

it is match all operators in python:
+       -       *       **      /       //      %      @
<<      >>      &       |       ^       ~       :=
<       >       <=      >=      ==      !=
*/
OPERATORS ([=<>!]=?)|(>>)|(<<)|(\*\*)|(:=)|([-&^~\|%@+*\/])|(\/\/)

/*
Delimiters:
it is match all Delimiters in python:
(       )       [       ]       {       }
,       :       .       ;       @       =       ->
+=      -=      *=      /=      //=     %=      @=
&=      |=      ^=      >>=     <<=     **=
*/
Delimiters ([\(\)\{\}\[\],:;.@])|(([\+\-\*/%@&\|^])|((\*{2}|<{2}|>{2})))=|(->)
%%

%{
	if(isEmpty(myStack)) {
    push(&myStack,0);
  }
%}

^[ \t]*     {
              /*
               * Handle indentation as described in Python docs linked above.
               * Note that this pattern treats leading spaces and leading tabs
               * equivalently, which could cause some unexpected behavior if
               * they're combined in a single line.  For the purposes of this
               * project, that's OK.
               */
              if (peek(myStack) < (int)yyleng) {
                /*
                 * If the current indentation level is greater than the
                 * previous indentation level (stored at the top of the stack),
                 * then emit an INDENT and push the new indentation level onto
                 * the stack.
                 */
                
                push(&myStack,yyleng);
                printf("INDENT\n");
                return INDENT;
              } else {
                /*
                 * If the current indentation level is less than or equal to
                 * the previous indentation level, pop indentation levels off
                 * the stack until the top is equal to the current indentation
                 * level.  Emit a DEDENT for each element popped from the stack.
                 */
                while (!isEmpty(myStack) && peek(myStack) != (int)yyleng) {
                  pop(&myStack);
                   printf("DEDENT\n");
                   return DEDENT;
                }

                /*
                 * If we popped everythin g off the stack, that means the
                 * current indentation level didn't match any on the stack,
                 * which is an indentation error.
                 */
                 
                if (isEmpty(myStack)) {
                  printf("Error: Incorrect indentation on line %d"
                    ,yylineno);
                  return 1;
                }
              }
            }

^[^ \t\n]+  {
              /*
               * If we find a line that's not indented, pop all indentation
               * levels off the stack, and emit a DEDENT for each one.  Then,
               * call REJECT, so the next rule matching this token is also
               * applied.
               */
              while(peek(myStack) != 0) {
                pop(&myStack);
                printf("DEDENT\n");
                return DEDENT;
              }
              REJECT;
            }

\r?\n       {
                  printf("NEWLINE\n");
                   return NEWLINE;
            }

<<EOF>>     {
              /*
               * If we reach the end of the file, pop all indentation levels
               * off the stack, and emit a DEDENT for each one.
               */
              while(peek(myStack) != 0) {
                pop(&myStack);
                printf("DEDENT\n");
                return DEDENT;
              }

            }
\" 			{
    				BEGIN(STRING1);  // Transition to the STRING start condition when a double quote is encountered
    				string_literal_value = strdup("");  // Initialize the string literal value
			  }
			
			
<STRING1>[^\"\n\\]+ 	{
    				            string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + yyleng + 1);
                				strcat(string_literal_value, yytext);
			                }
			
			
<STRING1>\\\n 		{	//skip  
			            }


<STRING1>\\\" 	  {
                  string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\"");  // Handle escaped double quote			
                  }
<STRING1>\" 	{
    				    printf("LITERAL_STRING : %s\n", string_literal_value);
                return STRING;
    				    BEGIN(INITIAL);  // Return to the initial start condition when a closing double quote is encountered
    				
			}





\' 			{
    				BEGIN(STRING2);  // Transition to the STRING start condition when a double quote is encountered
    				string_literal_value = strdup("");  // Initialize the string literal value
			}
			
			
<STRING2>[^\'\n\\]+ 	{
    			              string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + yyleng + 1);
                				strcat(string_literal_value, yytext);
			}
			
			
<STRING2>\\\n 		{	//string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);strcat(string_literal_value, "\n");  
			}


<STRING2>\\\' 		{
    					string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\'");  // Handle escaped double quote		
			}


<STRING2>\' 		{
    				    printf("LITERAL_STRING : %s\n", string_literal_value);
                return STRING;
    				    BEGIN(INITIAL);  // Return to the initial start condition when a closing double quote is encountered
			}





\"{3}    {
            BEGIN(STRING3);
    				string_literal_value = strdup("");  // Initialize the string literal value
        }

<STRING3>[^\\\"]+    {
                         string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + yyleng + 1);
                				strcat(string_literal_value, yytext);
        }

<STRING3>\\n    {
            string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\n");  // Handle escaped double quote		
        }

<STRING3>\\\"    {
            string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\"");  // Handle escaped double quote	
        }
        
<STRING3>\"    {
            string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\"");  // Handle escaped double quote	
        }

              
<STRING3>\\    {
            string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\\");  // Handle escaped double quote	
        }

                     
<STRING3>\'    {
            string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\'");  // Handle escaped double quote	
        }

<STRING3>\\\'    {
           string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\'");  // Handle escaped double quote	
        }

<STRING3>\\\\    {
            string_literal_value = realloc(string_literal_value, strlen(string_literal_value) + 1);
    				      strcat(string_literal_value, "\\");  // Handle escaped double quote	
        }

<STRING3>\"{3}    {
    				    printf("LITERAL_STRING : %s\n", string_literal_value);
            return STRING;
            BEGIN(INITIAL);
        }


        
"!="        { return NEQ; }
">"         { return GT; }
">="        { return GTE; }
"<"         { return LT; }
"<="        { return LTE; }

"("         { return yytext[0]; }
")"         { return yytext[0]; }

":"         { return COLON; }

"="  {printf("= Assign\n");return ASSIGN;}
"+" {return yytext[0];}
"-" {return yytext[0];}
"*" {return MUL;}
"**" {return yytext[0];}
"/" {return yytext[0];}
"//" {return yytext[0];}
"%" {return yytext[0];}
"@" {return yytext[0];}
"<<" {return LSHIFT;}
">>" {return RSHIFT;}
"&" {return yytext[0];}
"|" {return yytext[0];}
"^" {return yytext[0];}
"~" {return yytext[0];}
":=" {return yytext[0];}
"==" {return EQUAL;}
"[" {return yytext[0];}
"]" {return yytext[0];}
"{" {return yytext[0];}
"}" {return yytext[0];}
"," {return yytext[0];}
"." {return yytext[0];}
";" {return yytext[0];}
"->" {return yytext[0];}
"+=" {return yytext[0];}
"-=" {return yytext[0];}
"*=" {return yytext[0];}
"/=" {return yytext[0];}
"//=" {return yytext[0];}
"%=" {return yytext[0];}
"@=" {return yytext[0];}
"&=" {return yytext[0];}
"|=" {return yytext[0];}
"^=" {return yytext[0];}
">>=" {return yytext[0];}
"<<="  {return yytext[0];}
"**=" {return yytext[0];}

"if" { return IF; }
"else" { return ELSE; }
"elif" { return ELIF; }
"while" { return WHILE; }
"for" { return FOR; }
"def" { return DEF; }
"class" { return CLASS; }
"as" { return AS; }
"is" { return IS; }
"assert" { return ASSERT; }
"continue" { return CONTINUE; }
"break" { return BREAK; }
"del" { return DEL; }
"except" { return EXCEPT; }
"import" { return IMPORT; }
"in" { return IN; }
"lambda" { return LAMBDA; }
"print" { return PRINT; }
"finally" { return FINALLY; }
"global" { return GLOBAL; }
"not" { return NOT; }
"return" { return RETURN; }
"True" { return TRUE; }
"with" { return WITH; }
"yield" { return YIELD; }
"False" { return FALSE; }
"await" { return AWAIT; }
"pass" { return PASS; }
"raise" { return RAISE; }
"None" { return NONE; }
"and" { return AND; }
"try" { return TRY; }
"from" { return FROM; }
"nonlocal" { return NONLOCAL; }
"async" { return ASYNC; }
"or" { return OR; }
"match" {return MATCH;}
"case" {return CASE;}
{IDENTI}           		{printf("IDENTIFIER = %s in line = %d\n",yytext,yylineno); return IDENTIFIER;}
{NUMBER}                    {printf("number = %s in line = %d\n",yytext,yylineno); return NUMBER;}


#.*$        				{	printf("COMMENTS3: %s in line = %d\n", yytext,yylineno); /* Skip comments on the same line as a statement. */ }

^\"{3}    {
                BEGIN(COMMENT);
            
        }
<COMMENT>.      {}

<COMMENT>\"{3}    {
            BEGIN(INITIAL);
        }
%%

