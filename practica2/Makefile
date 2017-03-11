CC = gcc -g -Wall -ansi -pedantic -O3 -lm -lpthread -pthread
EXE = ejercicio3b

all : $(EXE)

$(EXE) : % : %.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -o  $@ $@.c -lm

.PHONY : clean
clean :
	rm -f *.o core $(EXE)
