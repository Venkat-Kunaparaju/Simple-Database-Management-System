CCo = g++ -o
CCc = g++ -c
WARNINGS = -Wno-deprecated
LEX = lex -l -o
YACC = yacc -d -o

all: engine


y.tab.o: engine.y
	$(YACC) y.tab.cc engine.y
	$(CCc) $(WARNINGS) y.tab.cc

lex.yy.o: engine.l
	$(LEX) lex.yy.cc engine.l
	$(CCc) $(WARNINGS) lex.yy.cc


engine: y.tab.o lex.yy.o 
	$(CCo) engine y.tab.o lex.yy.o 

.PHONY: clean
clean:
	rm -f table.o
	rm -f engine
	rm -f lex.yy.cc
	rm -f lex.yy.o



