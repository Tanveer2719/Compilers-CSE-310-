bison -d -t 1905025_Syntax_and_Semantic_Analyzer.y
echo "y.tab.h and y.tab.c created."
flex 1905025_Lexical_Analyzer.l
echo "lex.yy.c created."
g++ -fpermissive lex.yy.c y.tab.c -lfl -o parser.out
echo "compilation completed. parser.out is ready to execute."
./parser.out text.txt
echo "parser.out executed."