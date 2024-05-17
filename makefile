build:
	gcc -o main main.c array_list.c child_code.c -pthread
run:
	./main
all:	build run