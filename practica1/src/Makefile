CC = gcc -g -Wall -ansi -pedantic
EXE = ejercicio4a ejercicio4b  ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9

all : $(EXE)

$(EXE) : % : %.c
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -o  $@ $@.c

.PHONY : clean
clean :
		rm -f *.o core $(EXE)
