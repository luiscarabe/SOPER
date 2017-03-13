CC = gcc -g -Wall -ansi -pedantic 
EXE = ejercicio3a ejercicio3b ejercicio4a ejercicio4b ejercicio6 ejercicio8 prueba8

all : $(EXE)

.PHONY : clean

$(EXE) : % : %.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -o  $@ $@.c -lm -lpthread


clean :
	rm -f *.o core $(EXE)


