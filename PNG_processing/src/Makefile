all: app

app: main.o PNGio.o render.o
	gcc main.o PNGio.o render.o -o app -lpng

main.o: main.c PNGio.h render.h structs.h
	gcc -c main.c -lpng

render.o: render.c structs.h
	gcc -c render.c -lpng

PNGio.o: PNGio.c structs.h
	gcc -c PNGio.c -lpng

clean:
	rm -rf *.o app 