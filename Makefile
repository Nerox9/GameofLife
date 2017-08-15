CC=gcc
CFLAGS=-I.
LDFLAGS = -lncurses
DEPS = init.h
OBJ = main.o init.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

GoL: $(OBJ)
	gcc -o $@ $^ $(LDFLAGS) $(CFLAGS)

clean:
	rm -f $(OBJ) GoL
