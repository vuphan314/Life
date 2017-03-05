#include "game.h"

int main(int argc, const char *argv[]) {
  Char order = atoi(argv[1]);
  Game game(order);
  game.getImageProportion();
}
