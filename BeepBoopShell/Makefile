all: clean shell

shell:
	gcc -std=c99 -Wall -pedantic main.c scanner.c shell.c builtin.c bonus.c history.c processing.c -o shell

bonus:
	gcc -std=c99 -Wall -pedantic main.c scanner.c shell.c builtin.c bonus.c history.c processing.c -o shell -DEXT_PROMPT

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
