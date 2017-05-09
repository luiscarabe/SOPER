
CC = gcc -g -Wall -pedantic
EXE = cadena_montaje


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
