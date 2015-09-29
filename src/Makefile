CFLAGS=-ggdb -Wall

all: mymalloctesting 

debug: mymalloctesting
	gdb ./mymalloctesting

run: mymalloctesting
	./mymalloctesting | indent | dot -Tpng -o /tmp/outfile.png 

shell: shell.o map.o

map: map.o 

mymalloc: mymalloc.o 

mymalloctesting: mymalloc.o mymalloctesting.o  listgraph.o

listgraph: listgraph.o
	./listgraph | indent | dot -Tpng -o /tmp/outfile.png 

tags:
	ctags -R *.[ch]

clean:
	rm -f *.o shell 


