CFLAGS=-ggdb

all: shell

debug: shell
	gdb ./shell

run: shell
	./shell

shell: shell.o

clean:
	rm -f *.o shell
