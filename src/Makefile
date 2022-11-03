CCo = g++ -o
CCc = g++ -c
WARNINGS = -Wno-deprecated

all: table.o engine

table.o: table.cc
	$(CCc) $(WARNINGS) table.cc 
engine: table.o
	$(CCo) engine table.o

.PHONY: clean
clean:
	rm -f table.o
	rm -f engine



