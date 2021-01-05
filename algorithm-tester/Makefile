
# Makefile #


CXX=g++
SOURCES=main.o graph.o maxcut_algorithms.o
HEADERS=graph.h
EXE=maxcut
CXXFLAGS=--std=c++11
REMOVE=out.txt


all: clean ${EXE}

${EXE}: ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} -o $@ ${SOURCES}

debug: ${SOURCES} ${HEADERS}
	${CXX} -g ${CXXFLAGS} ${SOURCES}

clean:
	rm -f *.o ${EXE} ${REMOVE}


# $@ ··· [target]
# $^ ··· {Dependencies}

main.o: main.cpp
	${CXX} ${CXXFLAGS} -c main.cpp

graph.o: graph.cpp graph.h
	${CXX} ${CXXFLAGS} -c graph.cpp

maxcut_algorithms.o: maxcut_algorithms.cpp
	${CXX} ${CXXFLAGS} -c maxcut_algorithms.cpp

