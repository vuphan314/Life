#include "game.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    testGettingEdgeIndices();
  } else {
    Space space(atoi(argv[1]));
    setRULES();

    space.inspectPreImage();

    // space.isEachGrid4tupleJoinable();

    // space.isEachGrid3tupleJoinable();

    // space.isEachGrid2tupleJoinable();

    // space.getImageProportion();
  }
}
