CC = gcc -g -ansi -pedantic -lm -lpthread -pthread
EXE = ejercicio3a ejercicio3b ejercicio4a ejercicio4b

all : $(EXE)

.PHONY : clean

$(EXE) : % : %.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -o  $@ $@.c -lm


clean :
	rm -f *.o core $(EXE)


