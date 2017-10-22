CXX=g++
CFLAGS=
LFLAGS=-lm

all: calka.o
	${CXX} ${LFLAGS} -o calka calka.o

calka.o:
	${CXX} ${CFLAGS} -c calka.cpp

clean:
	rm -f calka calka.o

