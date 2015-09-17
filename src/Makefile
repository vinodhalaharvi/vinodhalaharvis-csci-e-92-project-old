CFLAGS=-ggdb

all: shell map

debug: shell
	gdb ./shell

run: shell
	./shell

shell: shell.o map.o

map: map.o 

clean:
	rm -f *.o shell 
