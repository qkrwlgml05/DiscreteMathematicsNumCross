run : cross.o
	gcc -o run cross.o
cross.o : cross.c
	gcc -c -o cross.o cross.c