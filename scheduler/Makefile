all: clean scheduler

scheduler:
	gcc -std=c99 -Wall -pedantic scheduler.c LibQueue.c LibList.c -o scheduler


clean:
	rm -f *~
	rm -f *.o
	rm -f scheduler
