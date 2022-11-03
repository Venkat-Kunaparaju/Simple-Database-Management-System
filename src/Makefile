CCo = g++ -o
CCc = g++ -c
CPPFLAGS:=-std=c++17 -W -Wall -pedantic -Werror

all: table.o engine


table.o: table.cc
	$(CCc) -Wno-deprecated table.cc 
engine: table.o
	$(CCo) engine table.o



