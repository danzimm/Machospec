CC=clang
CFLAGS=-Wall -Werror
OBJ=Machospec.c.o main.c.o

all: machospec

-include $(OBJ:.o=.d)

machospec: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
	$(CC) -MM $< $(CFLAGS) -o $(@:.o=.d)

