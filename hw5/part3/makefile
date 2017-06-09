all : main

main: main.c scheduler.c scheduler.h threadmap.c
	gcc -I ~/local/include switch.s queue.c threadmap.c async.c scheduler.c main.c -o main -lrt
spin: spin_test.c
	gcc -I ~/local/include spin_test.c -o spin_test
counter: counter_test.c scheduler.c scheduler.h threadmap.c
	gcc -I ~/local/include switch.s queue.c threadmap.c scheduler.c counter_test.c -o counter
