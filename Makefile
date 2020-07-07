all: test_threadpool
test_threadpool: main.o tpool.o
	gcc -pthread -o test_threadpool main.o tpool.o
main.o: main.c
	gcc -c main.c
tpool.o: tpool.c
	gcc -c tpool.c
clean:
	rm -rf test_threadpool main.o tpool.o
