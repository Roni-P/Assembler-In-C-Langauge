all: assembler

assembler: assembler.o file_handling.o symbol_table.o macrophase.o first_run.o second_run.o base32.o binaryInst.o binaryData.o Functions.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o file_handling.o symbol_table.o macrophase.o first_run.o second_run.o base32.o binaryInst.o binaryData.o Functions.o
  
file_handling.o: file_handling.c file_handling.h
	gcc -ansi -Wall -pedantic -c file_handling.c

assembler.o: assembler.c file_handling.h symbol_table.h macrophase.h first_run.h second_run.h base32.h binaryInst.h binaryData.h Functions.h
	gcc -ansi -Wall -pedantic -c assembler.c
	
symbol_table.o: symbol_table.c symbol_table.h
	gcc -ansi -Wall -pedantic -c symbol_table.c

macrophase.o: macrophase.c macrophase.h
	gcc -ansi -Wall -pedantic -c macrophase.c

binaryData.o: binaryData.c binaryData.h
	gcc -ansi -Wall -pedantic -c binaryData.c

binaryInst.o: binaryInst.c binaryInst.h
	gcc -ansi -Wall -pedantic -c binaryInst.c

base32.o: base32.c base32.h
	gcc -ansi -Wall -pedantic -c base32.c 

first_run.o: first_run.c first_run.h
	gcc -ansi -Wall -pedantic -c first_run.c
	
second_run.o: second_run.c second_run.h
	gcc -ansi -Wall -pedantic -c second_run.c

Functions.o: Functions.c Functions.h
	gcc -ansi -Wall -pedantic -c Functions.c

clean:
	rm *.o*~ assembler
