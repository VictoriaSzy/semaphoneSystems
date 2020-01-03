all: control.o semaphone.o
	gcc -o control control.o
	gcc -o write semaphone.o

control.o:
	gcc -c control.c

semaphone.o:
	gcc -c semaphone.c
