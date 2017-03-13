COMPILER = g++
EXECUTABLE = life.out
FLAGS = -c -Ofast
OBJECTS = main.o game.o ../cplusplus/src/vectors.o

############################################################

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) -o $(EXECUTABLE) $(OBJECTS)

main.o: main.cc game.h
	$(COMPILER) $(FLAGS) main.cc

game.o: game.h game.cc ../cplusplus/src/vectors.h
	$(COMPILER) $(FLAGS) game.cc

############################################################

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

subsystem:
	$(MAKE) -C ../cplusplus/src/
