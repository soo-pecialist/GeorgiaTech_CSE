# Makefile for HPC CX 4220 / CSE 6220 Programming Assignment 0
CXX=mpic++

# activate for compiler optimizations:
CCFLAGS=-Wall -O3
LDFLAGS=

# set up google test
CCFLAGS += -g

# set up google test
GTEST_DIR = ./gtest
CCFLAGS += -I. #-I$(GTEST_DIR)

all: grader_cal

test: tests
	echo "### TESTING SEQUENTIAL CODE ###";./seq_tests; \
	echo "### TESTING WITH 4 PROCESSES ###"; mpirun -np 4 ./mpi_tests \
	echo "### TESTING WITH 9 PROCESSES ###"; mpirun -np 9 ./mpi_tests
	
tests: seq_tests mpi_tests
	
seq_tests: seq_tests.o mpi_gtest.o gtest-all.o score_analyser.o utils.o
	$(CXX) $(LDFLAGS) -o $@ $^ -lpthread 

mpi_tests: mpi_tests.o mpi_gtest.o gtest-all.o mpi_score_analyser.o utils.o
	$(CXX) $(LDFLAGS) -o $@ $^ -lpthread 
	
grader_cal: main.o score_analyser.o mpi_score_analyser.o
	$(CXX) $(LDFLAGS) -o $@ $^

gtest-all.o : $(GTEST_DIR)/gtest-all.cc $(GTEST_DIR)/gtest.h
	$(CXX) $(CCFLAGS) -c $(GTEST_DIR)/gtest-all.cc
	
%.o: %.cpp %.c %.h
	$(CXX) $(CCFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CCFLAGS) -c $<

%.o: %.c
	$(CXX) $(CCFLAGS) -c $<


clean:
	rm -f *.o grader_cal seq_tests
