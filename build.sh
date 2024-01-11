flex pycompile.l
bison -d parser.y
gcc -o compiler parser.tab.c lex.yy.c