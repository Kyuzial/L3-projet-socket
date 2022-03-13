CC = gcc
CFLAGS = -g -Wall
BIN = main
SRCS = src/main.c

socket: $(SRCS)
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS)

clean:
	rm -f main
	rm -f vgcore.*
