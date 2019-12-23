all: control.o semaphone.o
	gcc control.o -o control
	gcc semaphone.o

control.o:
	gcc -c control.c

semaphone.o:
	gcc -c semaphone.c

setup:
	./control $(args)

run: all
	./a.out
