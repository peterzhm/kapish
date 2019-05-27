all: main.c
	gcc -g -Wall -pedantic-errors kapish.c -o kapish

clean:

	$(RM)	kapish


