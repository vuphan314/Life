OBJS = main.o game.o
CC = g++
CFLAGS = -c -Ofast

life.out: $(OBJS)
	$(CC) $(OBJS) -o life.out

main.o: main.cc game.h
	$(CC) $(CFLAGS) main.cc

game.o: game.h game.cc
	$(CC) $(CFLAGS) game.cc

clean:
	rm *.out *.o
