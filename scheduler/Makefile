all: clean scheduler

scheduler:
	gcc -g -std=c99 -Wall -pedantic scheduler.c LibQueue.c MyList.c -o scheduler


clean:
	rm -f *~
	rm -f *.o
	rm -f scheduler
