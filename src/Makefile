CFLAGS=-ggdb -Wall

all: shell map

debug: shell
	gdb ./shell

run: mymalloc
	./mymalloc

shell: shell.o map.o

map: map.o 

mymalloc: mymalloc.o 

clean:
	rm -f *.o shell 
