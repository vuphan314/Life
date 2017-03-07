#include "game.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cout << "Provide order in $1.\n";
  } else {
    Char order = atoi(argv[1]);
    Space game(order);
    // game.inspectSpace();
    game.showHorizontallyUnjoinableGridPair();
  }
}
