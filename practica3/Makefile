
CC = gcc -g -Wall -pedantic
EXE = ejercicio2 ejercicio5


all : $(EXE)

.PHONY : clean

ejercicio2: ejercicio2.c
	gcc -o $@ $^

semaforos.o: semaforos.c
	gcc -c $^

ejercicio5: ejercicio5.c semaforos.o
	gcc -o $@ $^


clean :
	rm -f *.o core $(EXE)
