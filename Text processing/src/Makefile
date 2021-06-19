all: main

main: main.o io.o render.o
	gcc main.o io.o render.o -o main

main.o: main.c io.h structs.h render.h
	gcc -c main.c

render.o: render.c structs.h
	gcc -c render.c

io.o: io.c io.h
	gcc -c io.c

clean:
	rm -rf *.o main