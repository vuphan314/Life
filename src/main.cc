#include "game.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cout << "Provide order in $1.\n";
  } else {
    Space space(atoi(argv[1]));
    setRULES();

    // space.inspectPreImage();

    // space.isEachGrid3tupleJoinable();

    space.setPreImage();
    bool success = space.are3wayJoinable(0, 0, 5, TRUE);
    cout << success << "\n";

    // space.isEachGrid3tuplePossiblyJoinable();

    // space.isEachGrid2tupleJoinable();

    // space.getImageProportion();
  }
}
