
# Makefile #


# $@ ··· [target]
# $^ ··· {Dependencies}


# Variables

EXE=maxcut
SOURCES=main.o graph.o maxcut_algorithms.o
HEADERS=graph.h

REMOVE=out.txt

CXX=g++
CXXFLAGS=--std=c++11


# Basic Commands

all: clean ${EXE}

${EXE}: ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} -o $@ ${SOURCES}

debug: ${SOURCES} ${HEADERS}
	${CXX} -g ${CXXFLAGS} ${SOURCES}

clean:
	rm -f *.o ${EXE} ${REMOVE}


# Individual Files

main.o: main.cpp
	${CXX} ${CXXFLAGS} -c main.cpp

graph.o: graph.cpp graph.h
	${CXX} ${CXXFLAGS} -c graph.cpp

maxcut_algorithms.o: maxcut_algorithms.cpp
	${CXX} ${CXXFLAGS} -c maxcut_algorithms.cpp

