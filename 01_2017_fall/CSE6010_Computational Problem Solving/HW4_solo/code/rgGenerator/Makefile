CC=gcc  #gcc-7 if interactive  mode on the local terminal
CFLAGS=  -fopenmp -std=gnu99 -Wall  -lpthread

all: rgGenerator paraTest
rgGenerator: rgGenerator.c
	$(CC) -o rgGenerator rgGenerator.c helpers/stack.c helpers/ganalyser.c graphgenerator.c $(CFLAGS)

paraTest: testParallel.c
	$(CC) -o paraTest testParallel.c helpers/stack.c helpers/ganalyser.c graphgenerator.c $(CFLAGS)

clean:
	rm ./rgGenerator
	rm  ./paraTest
