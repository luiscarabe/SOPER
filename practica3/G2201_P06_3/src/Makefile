
CC = gcc -g -Wall -pedantic
EXE = ejercicio2 ejercicio2b ejercicio5 ejercicio6


all : $(EXE)

.PHONY : clean

ejercicio2: ejercicio2.c
	gcc -o $@ $^


ejercicio2b: ejercicio2b.c
	gcc -o $@ $^


semaforos.o: semaforos.c
	gcc -c $^

ejercicio5: ejercicio5.c semaforos.o
	gcc -o $@ $^

ejercicio6: ejercicio6.c semaforos.o
	gcc -o $@ $^


clean :
	rm -f *.o core $(EXE)
