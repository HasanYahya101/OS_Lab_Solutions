all: main logger reader

main: main.c
	gcc -o main main.c

logger: logger.c
	gcc -o logger logger.c

receiver: reader.c
	gcc -o reader reader.c

clean:
	rm -f main logger reader
