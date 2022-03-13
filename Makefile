CC = gcc
CFLAGS = -g -Wall
BIN_CL = client.out
BIN_SV = server.out
SRCS_CL = src/client/client.c
SRCS_SV = src/server/server.c

client: $(SRCS_CL)
	$(CC) $(CFLAGS) -o $(BIN_CL) $(SRCS_CL)

server: $(SRCS_SV)
	$(CC) $(CFLAGS) -o $(BIN_SV) $(SRCS_SV)

clean:
	rm -f main
	rm -f vgcore.*
