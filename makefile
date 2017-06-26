all:main

main: main.c
	gcc -Wall main.c -o main -D DEBUG_

dist: main.c
	gcc -Wall main.c -o main

clean:
	rm *o main
