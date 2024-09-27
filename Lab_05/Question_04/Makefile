all: main sensor1 sensor2 sensor3

main: main.c
	gcc -o main main.c

sensor1: sensor1.c
	gcc -o sensor1 sensor1.c

receiver: sensor2.c
	gcc -o sensor2 sensor2.c

sensor3: sensor3.c
	gcc -o sensor3 sensor3.c

clean:
	rm -f main sensor1 sensor2 sensor3
