all: clean shell

shell:
	gcc -std=c99 -Wall -pedantic -g main.c scanner.c shell.c builtin.c bonus.c history.c -o shell

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
