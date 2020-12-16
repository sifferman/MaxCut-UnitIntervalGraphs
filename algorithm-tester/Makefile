
# Makefile #


CXX=g++
DEPENDENCIES=main.o graph.o maxcut_algorithms.o
EXE=maxcut
CXXFLAGS=--std=c++11
# REMOVE=

all: clean ${EXE}

${EXE}: ${DEPENDENCIES}
	${CXX} ${CXXFLAGS} -o $@ $^


debug: ${DEPENDENCIES}
	${CXX} -g ${CXXFLAGS} $^

# $@ ··· [target]
# $^ ··· {Dependencies}

main.o: main.cpp
	${CXX} ${CXXFLAGS} -c $^

graph.o: graph.cpp
	${CXX} ${CXXFLAGS} -c $^

maxcut_algorithms.o: maxcut_algorithms.cpp
	${CXX} ${CXXFLAGS} -c $^

clean:
	rm -f *.o ${EXE}
# ${REMOVE}
