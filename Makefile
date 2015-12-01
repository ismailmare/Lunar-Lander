#  Name: Ismail Mare
#  ID: 1388973
#  LECTURE SECTION: B1
#  UNIX ID: imare
#  INSTRUCTOR: Smith Jaqueline
#  LAB SECTION: H02
#  TA'S NAME: Nicholas Barriga
#
# build an executable, provide a make clean, a tar converter,
# and a submit target

lander: main.o draw_landscape.o draw_ship.o collision.o	
	gcc -Wall -std=c99 -g -o lander main.o draw_landscape.o draw_ship.o collision.o -lm -lcurses
main.o: main.c main.h
	gcc -Wall -std=c99 -c main.c 

draw_landscape.o: draw_landscape.c draw_landscape.h main.h
	gcc -Wall -std=c99 -c draw_landscape.c

draw_ship.o: draw_ship.c draw_ship.h main.h
	gcc -Wall -std=c99 -c draw_ship.c

collision.o: collision.c collision.h
	gcc -Wall -std=c99 -c collision.c

clean:
	-rm -f *.o lander core

tar:
	tar -cvf submit.tar Makefile README main.c main.h draw_ship.c draw_ship.h draw_landscape.c draw_landscape.h collision.c collision.h 

submit:
	astep -c c201 -p asn3 submit.tar
