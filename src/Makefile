CCo = g++ -o
CCc = g++ -c
WARNINGS = -Wno-deprecated
LEX = lex -l -o

all: engine

table.o: table.cc
	$(CCc) $(WARNINGS) table.cc 
lex.yy.o: engine.l
	$(LEX) lex.yy.cc engine.l
	$(CCc) $(WARNINGS) lex.yy.cc

engine: table.o lex.yy.o
	$(CCo) engine table.o lex.yy.o

.PHONY: clean
clean:
	rm -f table.o
	rm -f engine
	rm -f lex.yy.cc
	rm -f lex.yy.o



