#include "game.h"

////////////////////////////////////////////////////////////

Float getImageProportion(Char order) {
  Float codomainSize = getGridStateCount(order - 2);
  return getImageSize(order) / codomainSize;
}

Long getImageSize(Char order) {
  Char imageOrder = order - 2;
  Long codomainSize = getGridStateCount(order - 2);
  Image image(codomainSize, false);
  setImage(image, order);
  Long imageSize = 0;
  for (Long gridStateIndex = 0;
      gridStateIndex < codomainSize; gridStateIndex++) {
    if (image[gridStateIndex]) {
      imageSize++;
    } else {
      cout << "Not in image: grid state index " <<
        gridStateIndex << ".\n";
    }
  }
  cout << "Codomain size: " << codomainSize << ".\n";
  cout << "Image size: " << imageSize << ".\n";
  return imageSize;
}

void setImage(Image &image, Char order) {
  cout << "Start setting image.\n";
  Char imageOrder = order - 2;
  Grid grid(order, Row(order, false)),
    nextGrid(imageOrder, Row(imageOrder, false));
  Long gridStateCount = getGridStateCount(order);
  for (Long gridStateIndex = 0;
      gridStateIndex < gridStateCount; gridStateIndex++) {
    if (gridStateIndex % COUT_PERIOD == 0) {
      Int percent = 100 * gridStateIndex / gridStateCount;
      cout << "Grid state index" <<
        COUT_WIDTH << gridStateIndex <<
        " of" << COUT_WIDTH << gridStateCount <<
        COUT_WIDTH << percent << "%\n";
    }
    setGrid(grid, gridStateIndex);
    setNextGrid(nextGrid, grid);
    Long nextGridStateIndex = getGridStateIndex(nextGrid);
    image[nextGridStateIndex] = true;
  }
  cout << "End setting image.\n";
}

Long getGridStateCount(Char order) {
  Long gridStateCount = pow(2, pow(order, 2));
  return gridStateCount;
}

void setGrid(Grid &grid, Long gridStateIndex) {
  Char order = grid.size();
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      Char leastBit = gridStateIndex & 1;
      grid[ri][ci] = leastBit;
      gridStateIndex >>= 1;
    }
  }
}

Long getGridStateIndex(const Grid &grid) {
  Long gridStateIndex = 0;
  Int cellStateIndex = 0;
  Char order = grid.size();
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      gridStateIndex += grid[ri][ci] << cellStateIndex;
      cellStateIndex++;
    }
  }
  return gridStateIndex;
}

void setNextGrid(Grid &nextGrid, const Grid &grid) {
  Char order = grid.size();
  for (Char ri = 1; ri < order - 1; ri++) {
    for (Char ci = 1; ci < order - 1; ci++) {
      CellState nextState = getNextCellState(grid, ri, ci);
      nextGrid[ri - 1][ci - 1] = nextState;
    }
  }
}

CellState getNextCellState(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  Char count = getAliveNeighborCount(grid,
    rowIndex, columnIndex);
  if (isAlive(grid, rowIndex, columnIndex)) {
    return LOWER_SURVIVAL <= count &&
      count <= UPPER_SURVIVAL;
  } else {
    return LOWER_BIRTH <= count && count <= UPPER_BIRTH;
  }
}

Char getAliveNeighborCount(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  Char count = 0;
  char deltas[] = {-1, 0, 1};
  for (char rowDelta : deltas) {
    Char ri = rowIndex + rowDelta;
    for (char columnDelta : deltas) {
      Char ci = columnIndex + columnDelta;
      if (!(ri == rowIndex && ci == columnIndex) &&
          isAlive(grid, ri, ci)) {
        count++;
      }
    }
  }
  return count;
}

CellState isAlive(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  return grid[rowIndex][columnIndex];
}
