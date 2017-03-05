#include "game.h"

int main(int argc, const char *argv[]) {
  Game game;
  Char order = atoi(argv[1]);
  game.getImageProportion(order);
}
