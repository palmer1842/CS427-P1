wsucrypt: main.o keyschedule.o ffunc.o gperm.o
	gcc -o wsucrypt main.o keyschedule.o ffunc.o gperm.o

main.o: main.c
	gcc -c main.c

keyschedule.o: keyschedule.c
	gcc -c keyschedule.c

ffunc.o: ffunc.c
	gcc -c ffunc.c

gperm.o: gperm.c
	gcc -c gperm.c

clean:
	rm wsucrypt main.o keyschedule.o ffunc.o gperm.o
