#include "game.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cout << "Provide order in $1.\n";
  } else {
    setRULES();
    Char order = atoi(argv[1]);
    Space space(order);
    // space.inspectPreImage();
    // space.isEachGrid3tupleJoinable();
    space.isEachGrid3tuplePossiblyJoinable();
    // space.isEachGrid2tupleJoinable();
    // space.getImageProportion();
  }
}
