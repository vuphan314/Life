#include "game.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    testGettingEdgeIndices();
  } else {
    Space space(atoi(argv[1]));
    setRULES();

    // space.inspectPreImage();

    space.isEachGrid4tupleJoinable();

    // space.setEdgeMapPreImages();
    // Long gi0, gi1, gi2, gi3;
    // gi0 = gi1 = gi2 = gi3 = space.SPACE_SIZE - 1;
    // if (space.are4wayJoinable(gi0, gi1, gi2, gi3)) {
    //   cout << "yes\n";
    // } else {
    //   cout << "no\n";
    // }

    // space.isEachGrid3tupleJoinable();

    // space.isEachGrid2tupleJoinable();

    // space.getImageProportion();
  }
}
