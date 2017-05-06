BASE=gol5x5
C=$BASE.c
EXECUTABLE=$BASE.out

gcc -o $EXECUTABLE $C -lpthread -Ofast

./$EXECUTABLE $1
