CC = gcc
EXE = ejercicio4a ejercicio4a2 ejercicio4b ejercicio4b2  ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9

all : $(EXE)

.PHONY : clean

clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.c
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.c
