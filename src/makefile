OBJS = main.o game.o ../cplusplus/src/vectors.o
CC = g++
CFLAGS = -c -Ofast

life.out: $(OBJS)
	$(CC) $(OBJS) -o life.out

main.o: main.cc game.h ../cplusplus/src/vectors.h
	$(CC) $(CFLAGS) main.cc

game.o: game.h game.cc ../cplusplus/src/vectors.h
	$(CC) $(CFLAGS) game.cc

clean:
	rm *.out *.o

subsystem:
	$(MAKE) -C ../cplusplus/src/
