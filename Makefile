CC=clang
CFLAGS=-Wall -Werror -g -O0
OBJ=Machospec.c.o main.c.o

.PHONY: all clean

all: machospec

-include $(OBJ:.o=.d)

machospec: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
	$(CC) -MM $< $(CFLAGS) -o $(@:.o=.d)

clean:
	rm *.d *.o machospec

