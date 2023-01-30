yacc -d -y 1905025_Syntax_and_Semantic_Analyzer.y
g++ -w -c -o y.o y.tab.c
flex 1905025_Lexical_Analyzer.l
g++ -w -c -o l.o lex.yy.c
g++ y.o l.o -lfl -o text0
./text0 text.txt