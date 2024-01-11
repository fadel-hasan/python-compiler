# flex-bison python compiler

compiler for python made using lex yacc / [GNU's flex bison](https://www.gnu.org/software/bison/manual/).

The current version of this code supports `defining a function`, `defining a class`, `calling a function`, various types of `expressions`, `assignment` statements, and `if conditions`, in addition to loops such as `for` and `while` and some other structures such as `try`, `match`, `yield`, `global`, `nonlocal`, `return`, `break`, `continue`, `INDENT` \ `DEDENT` analysis using `stack`

## How to make
 You will need `flex`, `bison`, and `gcc` installed on your machine



#### To change sh script permission:
`$ chmod +x build.sh clear.sh`



#### To build:
`$ ./build.sh`
<br>
*compiled*: `compiler`, `parser.tab.c`, `parser.tab.h`, `lex.yy.c`




#### To run:
`$ ./compiler test.py`



#### To clear:
`$ ./clear.sh`


### Explaining the operating steps without using the build file

This program is run using the following commands:

```bash
bison -d parser.y 
```
- This instruction produces two files:
  - parser.tab.h : We use this to include it inside the flex file to read the token
  - parser.tab.c : We use this file to make a compiler with the resulting flex file

```bash
flex pycompile.l 
```
- This instruction produces lex.yy.c file: We use this file to make a compiler with the resulting bison file(parser.tab.c)

`Finally`, to compile flex and bixon, we write this command:

```bash
 gcc -o <program file name> parser.tab.c lex.yy.c
```
- This produces <name>.exe
<br>
`Note`: The `stack` file is included in order to deal with indentation in a file pycompile.l
test file is : test py

but now is ready to execution ^_____^

## Authors

- [@fadel-hasan](https://www.github.com/fadel-hasan)


## License

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
