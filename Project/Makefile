QUERIES = queries.o
BENCHMARK = benchmark.o
MAIN = main.o
DEFINES = NONE # DEBUG to print the B+tree
CXX = g++
CXXFLAGS = -std=c++14 -Iinclude -D $(DEFINES) -Wall -Wextra

# eliminate default suffixes
.SUFFIXES:
	SUFFIXES=

# just consider our own suffixes
.SUFFIXES: .cc .o

all: $(QUERIES) $(BENCHMARK) $(MAIN)

.PHONY: all

documentation: Doxygen/doxy.in
				doxygen $^

$(QUERIES): src/queries.cc
				$(CXX) -g $< -o $(QUERIES) $(CXXFLAGS)

$(BENCHMARK): src/benchmark.cc
				$(CXX) -g $< -o $(BENCHMARK) $(CXXFLAGS)

$(MAIN): 	src/main.cc
				$(CXX) -g $< -o $(MAIN) $(CXXFLAGS)

clean:
				rm -rf *.o *.o.dSYM html latex

.PHONY: clean documentation
