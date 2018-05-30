CC=gcc
CFLAGS=  -Wall

all: performancetest_static performancetest_dynamic
performancetest_static: performancetest_static.c
	$(CC) -o performancetest_static performancetest_static.c ../calendarqueue/calendarqueue.c  ../linearqueue/linearqueue.c $(CFLAGS)

performancetest_dynamic: performancetest_dynamic.c
	$(CC) -o performancetest_dynamic performancetest_dynamic.c ../calendarqueue/calendarqueue.c  ../linearqueue/linearqueue.c $(CFLAGS)


clean:
	rm ./performancetest_dynamic
	rm ./performancetest_static