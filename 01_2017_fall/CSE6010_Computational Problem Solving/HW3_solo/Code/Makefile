CC = gcc
CFLAGS = -g
LIBS = .
SRC = main.c application.c ./simengine/engine.c ./calendarqueue/calendarqueue.c
OBJ = $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

appln:
	$(CC) $(CFLAGS) main.c application.c ./calendarqueue/calendarqueue.c ./simengine/engine.c -o appln -lm

clean :
	@rm appln
	@echo Cleaned!
