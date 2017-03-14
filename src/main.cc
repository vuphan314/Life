#include "game.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cout << "Provide order in $1.\n";
  } else {
    setRULE_MATRIX();
    Char order = atoi(argv[1]);
    Space space(order);
    // space.inspectSpace();
    // space.isEachGrid3tupleJoinable();
    space.setPreImage();
    cout << "Joinable? " <<
      static_cast<bool>(space.are3wayJoinable(0, 0, 7)) << ".\n";
    // space.isEachGrid2tupleJoinable();
    // space.getImageProportion();
  }
}
