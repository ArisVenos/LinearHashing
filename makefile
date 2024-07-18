mvote: main.o hashing.o menu.o linkedlist.o
	gcc -g -o mvote main.o hashing.o menu.o linkedlist.o

main.o: main.c hashing.h	
	gcc -c main.c

hashing.o: hashing.c hashing.h 
	gcc -c hashing.c

menu.o: menu.c hashing.h
	gcc -c menu.c

linkedlist.o: linkedlist.c hashing.c
	gcc -c linkedlist.c

clean:
	rm -f mvote *.o