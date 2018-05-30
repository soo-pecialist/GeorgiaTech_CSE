CC=gcc
CFLAGS=  -Wall

simapp: simapp.c

	$(CC) -o simapp simapp.c ../calendarqueue/calendarqueue.c  ../simengine/engine.c $(CFLAGS)

clean:
	rm ./simapp